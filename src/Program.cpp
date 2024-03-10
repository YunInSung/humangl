#include "Program.hpp"

using namespace std;

Program::Program() {
	this->programId = 0;
	for (int i = 0; i < 6; i++)
		this->shaderId[i] = 0;
}

Program::~Program() {
}

void Program::erase() {
	this->clearShader();
	glDeleteProgram(this->programId);
	this->programId = 0;
}

void Program::loadVertex(std::string const &filename) {
	if (this->shaderId[0]!= 0)
		glDeleteShader(this->shaderId[0]);
	this->shaderId[0] = this->loadShader(GL_VERTEX_SHADER, filename);
}

void Program::loadTessControl(std::string const &filename) {
	if (this->shaderId[1]!= 0)
		glDeleteShader(this->shaderId[1]);
	this->shaderId[1] = this->loadShader(GL_TESS_CONTROL_SHADER, filename);
}

void Program::loadTessEvaluation(std::string const &filename) {
	if (this->shaderId[2]!= 0)
		glDeleteShader(this->shaderId[2]);
	this->shaderId[2] = this->loadShader(GL_TESS_EVALUATION_SHADER, filename);
}

void Program::loadGeometry(std::string const &filename) {
	if (this->shaderId[3]!= 0)
		glDeleteShader(this->shaderId[3]);
	this->shaderId[3] = this->loadShader(GL_GEOMETRY_SHADER, filename);
}

void Program::loadFragment(std::string const &filename) {
	if (this->shaderId[4]!= 0)
		glDeleteShader(this->shaderId[4]);
	this->shaderId[4] = this->loadShader(GL_FRAGMENT_SHADER, filename);
}

void Program::loadCompute(std::string const &filename) {
	if (this->shaderId[5]!= 0)
		glDeleteShader(this->shaderId[5]);
	this->shaderId[5] = this->loadShader(GL_COMPUTE_SHADER, filename);
}


GLuint Program::loadShader(GLenum type, std::string const &filename) {
	fstream file;
	file.open(filename);
	if (!file.is_open()) {
		cerr << filename << " not open" << endl;
		return 0;
	}
	string str;
	file.seekg(0,ios::end);
	size_t size = file.tellg();
	str.resize(size);
	file.seekg(0, ios::beg);
	file.read(&str[0], size);
	file.close();

	GLuint id = glCreateShader(type);
	const GLchar *source = str.c_str();
	const GLint length = str.length();
	glShaderSource(id, 1, &source, &length);

	glCompileShader(id);

	GLint check;
	glGetShaderiv(id, GL_COMPILE_STATUS, &check);
	if (check == GL_FALSE) {
		glDeleteShader(id);
		cerr << filename << " not compiled" << endl;
		return 0;
	};

	return id;
}

void Program::clearShader() {
	for (int i = 0; i < 6; i++) {
		if (this->shaderId[i]) {
			glDeleteShader(this->shaderId[i]);
			this->shaderId[i] = 0;
		}
	}
}

bool Program::loadProgram() {
	GLuint id = glCreateProgram();
	// put shader in program
	for (int i = 0; i < 6; i++) {
		if (this->shaderId[i])
			glAttachShader(id, this->shaderId[i]);
	}
	// link program
	glLinkProgram(id);

	for (int i = 0; i < 6; i++) {
		if (this->shaderId[i])
			glDetachShader(id, this->shaderId[i]);
	}

	// check link done
	GLint check;
	glGetProgramiv(id, GL_LINK_STATUS, &check);
	if (check == GL_FALSE) {
		glDeleteProgram(id);
		cout << "Program does not linked" << endl;
		return false;
	}

	this->clearShader();

	this->programId = id;
	return true;
}

GLuint Program::id() {
	return this->programId;
}

bool Program::is_set() {
	if (this->programId)
		return true;
	return false;
}

void Program::use() {
	glUseProgram(this->programId);
}

void Program::setBool(std::string const &name, bool value) {         
	glUniform1i(glGetUniformLocation(programId, name.c_str()), (int)value); 
}
void Program::setInt(std::string const &name, int value) { 
	glUniform1i(glGetUniformLocation(programId, name.c_str()), value); 
}
void Program::setFloat(std::string const &name, float value) { 
	glUniform1f(glGetUniformLocation(programId, name.c_str()), value); 
}
void Program::setVec2(std::string const &name, glm::vec2 value) { 
	glUniform2fv(glGetUniformLocation(programId, name.c_str()), 1, &value[0]); 
}
void Program::setVec2(std::string const &name, float x, float y) { 
	glUniform2f(glGetUniformLocation(programId, name.c_str()), x, y); 
}
void Program::setVec3(std::string const &name, glm::vec3 value) { 
	glUniform3fv(glGetUniformLocation(programId, name.c_str()), 1, &value[0]); 
}
void Program::setVec3(std::string const &name, float x, float y, float z) { 
	glUniform3f(glGetUniformLocation(programId, name.c_str()), x, y, z); 
}
void Program::setVec4(std::string const &name, glm::vec4 value) { 
	glUniform4fv(glGetUniformLocation(programId, name.c_str()), 1, &value[0]); 
}
void Program::setVec4(std::string const &name, float x, float y, float z, float w)  { 
	glUniform4f(glGetUniformLocation(programId, name.c_str()), x, y, z, w); 
}
void Program::setMat3(std::string const &name, glm::mat3 &mat) {
	glUniformMatrix3fv(glGetUniformLocation(programId, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
void Program::setMat4(std::string const &name, glm::mat4 &mat) {
	glUniformMatrix4fv(glGetUniformLocation(programId, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}