#include "Skeleton.hpp"

Skeleton::Skeleton(): error(false), isSpecified(false) {};

Skeleton::~Skeleton() {};

// parse ASF file
bool Skeleton::setASF(std::string const &filename) {
	this->ASF_Filename = filename;

	char ch;
	std::fstream file;

	file.open(this->ASF_Filename);
	if (!file.is_open())
		exit(1);
	while (!file.eof()) {
		if (this->error) {
			file.close();
			return false;
		}
		skipWhiteSpace(file);

		ch = file.get();
		if (file.eof() || ch < 0)
			break;
		else if (ch == '#') {
			std::string trash;
			getline(file, trash);
			continue;
		} else if (ch == ':') {
			std::string buffer;
			file >> buffer;

			if (buffer == "version")
				file >> this->version;
			else if (buffer == "name")
				file >> this->name;
			else if (buffer == "documentation") {
				std::string str;
				getline(file, str);
				getline(file, str);
				this->documentation.append(str);
				getline(file, str);
				this->documentation.append(str);
			} else if (buffer == "units")
				this->error = !this->setUnit(file);
			else if (buffer == "root")
				this->error = !this->setRoot(file);
			else if (buffer == "bonedata")
				this->error = !this->setBoneData(file);
			else if (buffer == "hierarchy")
				this->error = !this->setHierarchy(file);
		} else {
			std::cout << "부적적한 ASF 파일" << std::endl;
			file.close();
			return false;
		}
	}
	file.close();

	return true;
}

bool Skeleton::setUnit(std::fstream &file) {
	std::string buffer;
	bool check[3] = {false, false, false};

	for (int i = 0; i < 3; i++) {
		file >> buffer;
		if (buffer == "mass") {
			check[0] = true;
			file >> this->mass;
		} else if (buffer == "length") {
			check[1] = true;
			file >> this->length;
		} else if (buffer == "angle") {
			check[2] = true;
			file >> buffer;
			if (buffer == "deg") {
				this->ASFisRad = false;
				this->AMCisRad = false;
			} else if (buffer == "rad") {
				this->ASFisRad = true;
				this->AMCisRad = true;
			} else
				return false;
		} else
			return false;
	}

	for (int i = 0; i < 3; i++) {
		if (!check[i])
			return false;
	}
	return true;
}

bool Skeleton::setRoot(std::fstream &file) {
	Joint root;

	root.id = 0;
	root.name = "root";

	std::string buffer;
	bool check[4] = {false, false, false, false};
	
	for(int i = 0; i < 4; i++) {
		file >> buffer;
		if (buffer == "axis") {
			check[0] = true;
			file >> buffer;

			for (int i = 0; i < 3; i++)
				buffer[i] = ::toupper(buffer[i]);

			if (buffer == "XYZ")
				root.eulerOrder = EULER_XYZ;
			else if (buffer == "XZY")
				root.eulerOrder = EULER_XZY;
			else if (buffer == "YXZ")
				root.eulerOrder = EULER_YXZ;
			else if (buffer == "YZX")
				root.eulerOrder = EULER_YZX;
			else if (buffer == "ZXY")
				root.eulerOrder = EULER_ZXY;
			else if (buffer == "ZYX")
				root.eulerOrder = EULER_ZYX;
			else
				return false;

		} else if (buffer == "order") {
			check[1] = true;
			this->error = !this->setRootOrder(file, root);
		} else if (buffer == "position") {
			check[2] = true;
			for (int i = 0; i < 3; i++)
				file >> this->position[i];
		} else if (buffer == "orientation") {
			check[3] = true;
			for (int i = 0; i < 3; i++)
				file >> root.axis[i];
			if (this->ASFisRad == false) {
				for (int i = 0; i < 3; i++)
					root.axis[i] = glm::radians(root.axis[i]);
			}
		} else
			return false;
	}
	glm::mat4 c = eulerRotation(root.axis[0], root.axis[1], root.axis[2], root.eulerOrder);
	root._C = c;
	root._Cinv = glm::inverse(c);
	root.position = glm::vec3(0, 0, 0);

	for (int i = 0; i < 4; i++) {
		if (!check[i])
			return false;
	}
	this->joints.insert(std::make_pair("root", root));
	return true;
}

bool Skeleton::setRootOrder(std::fstream &file, Joint &root) {
	std::string buffer;

	for (int i = 0; i < 6; i++) {
		file >> buffer;

		std::transform(buffer.begin(), buffer.end(), buffer.begin(), ::tolower);
		if (buffer != "tx" && buffer != "ty" && buffer != "tz" && buffer != "rx" && buffer != "ry" && buffer != "rz")
			return false;
		root.dof.push_back(buffer);
	}
	return true;
}

bool Skeleton::setBoneData(std::fstream &file) {
	std::string buffer;

	glm::vec3 direction;
	float length;

	while (1) {
		if (file.eof() || file.peek() == ':')
			return true;
		
		file >> buffer;
		if (buffer != "begin")
			return false;
		
		Joint joint;
		std::string euler;
		while (1) {
			file >> buffer;
			if (buffer == "id")
				file >> joint.id;
			else if (buffer == "name")
				file >> joint.name;
			else if (buffer == "direction") {
				float value;
				for (int i = 0; i < 3; i++) {
					file >> value;
					direction[i] = value;
				}
				direction = glm::normalize(direction);
			} else if (buffer == "length")
				file >> length;
			else if (buffer == "axis") {
				float value;
				for (int i = 0; i < 3; i++) {
					file >> value;
					joint.axis[i] = value;
				}
				if (this->ASFisRad == false) {
					for (int i = 0; i < 3; i++)
						joint.axis[i] = glm::radians(joint.axis[i]);
				}
				file >> buffer;
				for (int i = 0; i < 3; i++)
					buffer[i] = ::toupper(buffer[i]);

				if (buffer == "XYZ")
					joint.eulerOrder = EULER_XYZ;
				else if (buffer == "XZY")
					joint.eulerOrder = EULER_XZY;
				else if (buffer == "YXZ")
					joint.eulerOrder = EULER_YXZ;
				else if (buffer == "YZX")
					joint.eulerOrder = EULER_YZX;
				else if (buffer == "ZXY")
					joint.eulerOrder = EULER_ZXY;
				else if (buffer == "ZYX")
					joint.eulerOrder = EULER_ZYX;

			} else if (buffer == "dof")
				this->error = !this->setBoneDataDof(file, joint);
			else if (buffer == "limits")
				this->error = !this->setBoneDataLimits(file, joint);
			else if (buffer == "end") {
				glm::mat4 c = eulerRotation(joint.axis[0], joint.axis[1], joint.axis[2], joint.eulerOrder);
				joint._C = c;
				joint._Cinv = glm::inverse(c);
				joint.position = this->length * length * direction;

				joint._ROT = joint._C * joint._Cinv;
				joint._B = joint.position;
				
				this->joints.insert(std::pair<std::string, Joint>(joint.name, joint));
				getline(file, buffer);
				break;
			} else
				return false;	
		}
	}

	return true;
}

bool Skeleton::setBoneDataDof(std::fstream &file, Joint &joint) {
	std::string line;
	std::string buffer;

	getline(file, line);

	std::stringstream ss(line);
	while (1) {
		ss >> buffer;
		if (ss.eof())
			break;

		std::transform(buffer.begin(), buffer.end(), buffer.begin(), ::tolower);
		if (buffer != "rx" && buffer != "ry" && buffer != "rz")
			return false;
		joint.dof.push_back(buffer);
	}
	return true;
}

bool Skeleton::setBoneDataLimits(std::fstream &file, Joint &joint) {
	std::string line;
	std::string buffer;
	float floatBuffer;

	while (1) {
		buffer = peekWord(file);
		if (buffer == "end") {
			if (joint.limits.size() % 2)
				return false;
			else
				return true;
		}
		getline(file, buffer);
		std::stringstream ss(trim(buffer));
		glm::vec2 lim;
		ss >> floatBuffer;
		if (this->ASFisRad == false)
			floatBuffer = glm::radians(floatBuffer);
		lim.x = floatBuffer;
		ss >> floatBuffer;
		if (this->ASFisRad == false)
			floatBuffer = glm::radians(floatBuffer);
		lim.y = floatBuffer;
		joint.limits.push_back(lim);
	}
}

bool Skeleton::setHierarchy(std::fstream &file) {
	std::string buffer;

	if (file.eof() || file.peek() == ':')
		return true;
	
	file >> buffer;
	if (buffer != "begin")
		return false;
	getline(file, buffer);
	
	while (1) {
		file >> buffer;
		if (buffer == "end")
			return true;
		std::string line;
		getline(file, line);

		std::stringstream ss(line);
		std::string parent = buffer;
		std::vector<std::string> childs;
		while (1) {
			ss >> buffer;
			if (ss.eof())
				break;
			childs.push_back(buffer);
		}
		this->hierarchy.insert(std::make_pair(parent, childs));
	}
	return true;;
}
// parse ASF file



// parse AMC file
bool Skeleton::setAMC(std::string const &filename) {
	this->AMC_Filename = filename;

	char ch;
	std::fstream file;

	file.open(this->AMC_Filename);
	if (!file.is_open())
		exit(1);
	while (!file.eof()) {
		if (this->error)
			return false;
		skipWhiteSpace(file);

		ch = file.peek();
		if (file.eof() || ch < 0)
			break;
		else if (ch == '#') {
			std::string trash;
			getline(file, trash);
			continue;
		} else if (ch == ':') {
			std::string buffer;
			file >> buffer;

			if (buffer == "FULLY-SPECIFIED")
				this->isSpecified = true;
			else if (buffer == "DEGREES")
				this->AMCisRad = false;
			else if (buffer == "RADIANS")
				this->AMCisRad = true;
			
			getline(file, buffer);
		} else if (isdigit(ch)) {
			KeyFrame frame;

			file >> frame.sequence;
			if (this->keyFrame.size() + 1 != frame.sequence) {
				this->error = true;
				return false;
			}
			frame.time = frame.sequence / FRAMERATE;
			this->keyFrame.push_back(frame);
		} else {
			std::string jointName;
			std::vector<float> motion;
			float buffer;
			std::string line;

			getline(file, line);
			std::stringstream ss(line);

			ss >> jointName;
			int rootIdx = 0;
			while (!ss.eof()) {
				ss >> buffer;
				// if (this->AMCisRad == false && (rootIdx > 2 && jointName == "root"))
				if (jointName == "root" && this->AMCisRad == false && rootIdx > 2)
					buffer = glm::radians(buffer);
				else if (jointName != "root" && this->AMCisRad == false)
					buffer = glm::radians(buffer);
				motion.push_back(buffer);
				rootIdx++;
			}

			std::map<std::string, Joint>::iterator joint = this->joints.find(jointName);
			// tx, ty, tz, rx, ry, rz 순서
			int ck[6] = {-1, -1, -1, -1, -1, -1};
			for (int i = 0; i < joint->second.dof.size(); i++) {
				if (joint->second.dof[i] == "tx")
					ck[0] = i;
				else if (joint->second.dof[i] == "ty")
					ck[1] = i;
				else if (joint->second.dof[i] == "tz")
					ck[2] = i;
				else if (joint->second.dof[i] == "rx")
					ck[3] = i;
				else if (joint->second.dof[i] == "ry")
					ck[4] = i;
				else if (joint->second.dof[i] == "rz")
					ck[5] = i;
			}
			std::vector<float> jointDof(6, 0);
			for (int i = 0; i < 6; i++) {
				if (ck[i] != -1)
					jointDof[i] = motion[ck[i]];
			}

			this->keyFrame.back().motions.insert(std::make_pair(jointName, jointDof));
		}
	}
	
	file.close();
	return true;
}
// parse AMC file



// for skeleton
bool Skeleton::setGlobalTransform() {
	std::stack<glm::mat4> rotateStack;
	std::stack<glm::vec3> translateStack;

	this->recursive("", "root", rotateStack, translateStack);
	return true;
}
void Skeleton::recursive(
	std::string const &parent,
	std::string const &name,
	std::stack<glm::mat4> rotateStack,
	std::stack<glm::vec3> translateStack) {

	std::map<std::string, Joint>::iterator joint = this->joints.find(name);
	
	rotateStack.push(joint->second._ROT);
	translateStack.push(joint->second._B);
	std::stack<glm::mat4> rotateCur = rotateStack;
	std::stack<glm::vec3> translateCur = translateStack;

	joint->second.cor = glm::vec3(this->position[0], this->position[1], this->position[2]);
	while (!rotateCur.empty() && !translateCur.empty()) {
		if (rotateCur.size() == 1 && translateCur.size() == 1) {
			// joint->second.cor = rotateCur.top() * glm::vec4(joint->second.cor, 1);
			glm::vec4 tmp = rotateCur.top() * glm::vec4(joint->second.cor, 1);
			joint->second.cor = glm::vec3(tmp.x, tmp.y, tmp.z);
			joint->second.cor = joint->second.cor + translateCur.top();
		} else {
			joint->second.cor = joint->second.cor + translateCur.top();
			// joint->second.cor = rotateCur.top() * glm::vec4(joint->second.cor, 1);
			glm::vec4 tmp = rotateCur.top() * glm::vec4(joint->second.cor, 1);
			joint->second.cor = glm::vec3(tmp.x, tmp.y, tmp.z);

		}
		rotateCur.pop();
		translateCur.pop();
	}

	if (!this->hierarchy.count(name))
		return;

	std::vector<std::string> childs = this->hierarchy.find(name)->second;
	for (std::vector<std::string>::iterator it = childs.begin(); it != childs.end(); it++)
		this->recursive(name, *it, rotateStack, translateStack);
}

bool Skeleton::setGlobalTransform(glm::vec3 &pos, glm::mat4 &look) {
	std::stack<glm::mat4> rotateStack;
	std::stack<glm::vec3> translateStack;

	this->recursive("", "root", rotateStack, translateStack, pos, look);
	return true;
}

void Skeleton::recursive(
	std::string const &parent,
	std::string const &name,
	std::stack<glm::mat4> rotateStack,
	std::stack<glm::vec3> translateStack,
	glm::vec3 &pos,
	glm::mat4 &look) {

	std::map<std::string, Joint>::iterator joint = this->joints.find(name);
	
	rotateStack.push(joint->second._ROT);
	translateStack.push(joint->second._B);
	std::stack<glm::mat4> rotateCur = rotateStack;
	std::stack<glm::vec3> translateCur = translateStack;

	joint->second.cor = glm::vec3(this->position[0], this->position[1], this->position[2]);
	while (!rotateCur.empty() && !translateCur.empty()) {
		if (rotateCur.size() == 1 && translateCur.size() == 1) {
			// joint->second.cor = rotateCur.top() * glm::vec4(joint->second.cor, 1);
			// joint->second.cor = look * glm::vec4(joint->second.cor, 1);
			glm::vec4 tmp = rotateCur.top() * glm::vec4(joint->second.cor, 1);
			tmp = look * tmp;
			joint->second.cor = glm::vec3(tmp.x, tmp.y, tmp.z);
			joint->second.cor = joint->second.cor + pos;
		} else {
			joint->second.cor = joint->second.cor + translateCur.top();
			// joint->second.cor = rotateCur.top() * glm::vec4(joint->second.cor, 1);
			glm::vec4 tmp = rotateCur.top() * glm::vec4(joint->second.cor, 1);
			joint->second.cor = glm::vec3(tmp.x, tmp.y, tmp.z);
		}

		rotateCur.pop();
		translateCur.pop();
	}

	if (!this->hierarchy.count(name))
		return;

	std::vector<std::string> childs = this->hierarchy.find(name)->second;
	for (std::vector<std::string>::iterator it = childs.begin(); it != childs.end(); it++)
		this->recursive(name, *it, rotateStack, translateStack, pos, look);
}

// for skeleton

void Skeleton::setLocal(float curTime) {
	while (curTime >= this->keyFrame.back().time)
		curTime -= this->keyFrame.back().time;

	KeyFrame *first, *second;
	first = (this->keyFrame.end() - 1).base();
	for (std::vector<KeyFrame>::iterator it = this->keyFrame.begin(); it != this->keyFrame.end(); it++) {
		if (it->time > curTime) {
			second = it.base();
			break;
		}
		first = it.base();
		second = it.base();
	}
	float t;
	if (second->sequence == this->keyFrame.begin()->sequence) {
		first = &this->keyFrame.back();
		t =  (second->time - curTime) / (second->time - 0);
	} else 
		t =  (second->time - curTime) / (second->time - first->time);
	t = std::min(std::max(t, 0.0f), 1.f);

	this->setJointLocal(*first, *second, t);
}

void Skeleton::setJointLocal(KeyFrame &first, KeyFrame &second, float t) {
	for (std::map<std::string, Joint>::iterator it = this->joints.begin(); it != this->joints.end(); it++) {

		if (first.motions.count(it->first) == 0)
			continue;
		if (second.motions.count(it->first) == 0)
			continue;
			
		std::vector<float> firstMo = first.motions.find(it->first)->second;
		std::vector<float> secondMo = second.motions.find(it->first)->second;

		if (it->first == "root") {
			glm::vec3 firstPos(firstMo[0], firstMo[1], firstMo[2]);
			glm::vec3 secondPos(secondMo[0], secondMo[1], secondMo[2]);
			it->second._B = glm::lerp(firstPos, secondPos, t) * this->length;
		} else
			it->second._B = it->second.position;

		glm::vec3 firstRot(firstMo[3], firstMo[4], firstMo[5]);
		glm::vec3 secondRot(secondMo[3], secondMo[4], secondMo[5]);
		glm::vec3 curRot = glm::lerp(firstRot, secondRot, t);
		glm::mat4 _M(eulerRotation(curRot.x, curRot.y, curRot.z, it->second.eulerOrder));

		it->second._ROT = it->second._C * _M * it->second._Cinv;
	}
}

std::vector<glm::vec3> Skeleton::getPoints() {
	std::vector<glm::vec3> res;

	for (std::map<std::string, Joint>::iterator it = this->joints.begin(); it != this->joints.end(); it++) {
		res.push_back(it->second.cor);
	}
	return res;
}

std::vector<glm::vec3> Skeleton::getLines() {
	std::vector<glm::vec3> res;

	this->recursiveLine("", "root", res);

	return res;
}

void Skeleton::recursiveLine(std::string const &parent, std::string const &name, std::vector<glm::vec3> &res) {
	if (parent != "") {
		Joint parentJoint = this->joints.find(parent)->second;
		Joint nameJoint = this->joints.find(name)->second;

		res.push_back(parentJoint.cor);
		res.push_back(nameJoint.cor);
	}

	if (!this->hierarchy.count(name))
		return;

	std::vector<std::string> childs = this->hierarchy.find(name)->second;
	for (std::vector<std::string>::iterator it = childs.begin(); it != childs.end(); it++)
		this->recursiveLine(name, *it, res);
}

std::vector<glm::mat4> Skeleton::getcube() {
	std::vector<glm::mat4> res;
	this->recursiveCube("", "root", res);
	return res;
}

void Skeleton::recursiveCube(std::string const &parent, std::string const &name, std::vector<glm::mat4> &res) {
	if (parent != "") {
		glm::vec3 pV = this->joints.find(parent)->second.cor;
		glm::vec3 nV = this->joints.find(name)->second.cor;
		glm::vec3 v = glm::normalize(pV - nV);

		glm::mat4 scale = glm::scale(glm::mat4(1), glm::vec3(glm::length(pV - nV), this->length, this->length));

		glm::mat4 trans = glm::translate(glm::mat4(1), (pV + nV) / 2.f);

		glm::vec3 axis = glm::cross(v, {1, 0, 0});
		float angle = acos(glm::dot(v, {1, 0, 0}));
		glm::mat4 rotate = glm::rotate(glm::mat4(1), -angle, axis);

		res.push_back(trans * rotate * scale);

	}

	if (!this->hierarchy.count(name))
		return;

	std::vector<std::string> childs = this->hierarchy.find(name)->second;
	for (std::vector<std::string>::iterator it = childs.begin(); it != childs.end(); it++)
		this->recursiveCube(name, *it, res);
}



static glm::mat4 eulerRotation  (float x, float y, float z, int order) {
	glm::mat4 X = glm::rotate(glm::mat4(1), x, glm::vec3(1, 0, 0));
	glm::mat4 Y = glm::rotate(glm::mat4(1), y, glm::vec3(0, 1, 0));
	glm::mat4 Z = glm::rotate(glm::mat4(1), z, glm::vec3(0, 0, 1));

	if (order == EULER_XYZ)
		return Z * Y * X;
	else if (order == EULER_XZY)
		return Y * Z * X;
	else if (order == EULER_YXZ)
		return Z * X * Y;
	else if (order == EULER_YZX)
		return X * Z * Y;
	else if (order == EULER_ZXY)
		return Y * X * Z;
	else if (order == EULER_ZYX)
		return X * Y * Z;

	return Z * Y * X;
}

static void skipWhiteSpace(std::fstream &file) {
	while (1) {
		char c = file.peek();
		if (std::isspace(c))
			file.get();
		else
			break;
	}
}

static std::string trim(std::string &str) {
	std::string res;

	for (std::string::iterator it = str.begin(); it != str.end(); it++) {
		if (*it == '(' || *it == ')')
			continue;
		res.push_back(*it);
	}
	return res;
}

static std::string peekLine(std::fstream &file) {
	std::string str;

	getline(file, str);
	file.seekg(str.length() * -1, std::ios_base::seekdir::cur);
	return str;
}

static std::string peekWord(std::fstream &file) {
	std::string str;

	file >> str;
	file.seekg(str.length() * -1, std::ios_base::seekdir::cur);
	return str;
}