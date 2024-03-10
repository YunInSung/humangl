#include "Object.hpp"

Object::Object() {
	this->model = glm::mat4(1);
	this->scaleMat = glm::mat4(1);
	this->rotateMat = glm::mat4(1);
	this->translateMat = glm::mat4(1);
	this->VAO = 0;
	this->VBO = 0;
}

Object::~Object() {
	glDeleteBuffers(1, &this->VBO);
	glDeleteVertexArrays(1, &this->VAO);
}

Object::Object(const Object& other) {}

Object& Object::operator=(const Object& other) {
	if (this == &other)
		return *this;
	return *this;
}

void Object::setVertex(const std::vector<glm::vec3>& v) {
	this->vertex = v;
}

void Object::setNormal(const std::vector<glm::vec3>& n) {
	this->normal = n;
}

void Object::setTexture(const std::vector<glm::vec2>& t) {
	this->texture = t;
}

void Object::setting() {
	if (this->vertex.size() == 0)
		return;
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);

	// std::cout << "Object VAO : " << this->VAO << " , " << "VBO : " << this->VBO << std::endl;

	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);

	glBufferData(GL_ARRAY_BUFFER,
				this->vertex.size() * sizeof(glm::vec3) + this->normal.size() * sizeof(glm::vec3) + this->texture.size() * sizeof(glm::vec2),
				0,
				GL_STATIC_DRAW);

	glBufferSubData(GL_ARRAY_BUFFER, 0, this->vertex.size() * sizeof(glm::vec3), this->vertex.data());
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

	if (this->normal.size() != 0) {
		glBufferSubData(GL_ARRAY_BUFFER,
						this->vertex.size() * sizeof(glm::vec3),
						this->normal.size() * sizeof(glm::vec3),
						this->normal.data());
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
							sizeof(glm::vec3),
							(void*)(this->vertex.size() * sizeof(glm::vec3)));
	}

	if (this->normal.size() == 0 && this->texture.size() != 0) {
		glBufferSubData(GL_ARRAY_BUFFER,
						this->vertex.size() * sizeof(glm::vec3),
						this->texture.size() * sizeof(glm::vec2),
						this->texture.data());
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
							sizeof(glm::vec2),
							(void*)(this->vertex.size() * sizeof(glm::vec3)));
	} else if (this->normal.size() != 0 && this->texture.size() != 0) {
		glBufferSubData(GL_ARRAY_BUFFER,
						this->vertex.size() * sizeof(glm::vec3) + this->normal.size() * sizeof(glm::vec3),
						this->texture.size() * sizeof(glm::vec2),
						this->texture.data());
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
							sizeof(glm::vec2),
							(void*)(this->vertex.size() * sizeof(glm::vec3) + this->normal.size() * sizeof(glm::vec3)));
	}
	glBindVertexArray(0);
}

void Object::normalMappingSetting() {
	if (this->vertex.size() == 0)
		return;

	for (int i = 0; i < this->vertex.size() / 3; i++) {
		glm::vec3 pos1 = this->vertex[i * 3];
		glm::vec3 pos2 = this->vertex[i * 3 + 1];
		glm::vec3 pos3 = this->vertex[i * 3 + 2];

		glm::vec2 uv1 = this->texture[i * 3];
		glm::vec2 uv2 = this->texture[i * 3 + 1];
		glm::vec2 uv3 = this->texture[i * 3 + 2];

		glm::vec3 edge1 = pos2 - pos1;
		glm::vec3 edge2 = pos3 - pos1;
		glm::vec2 deltaUV1 = uv2 - uv1;
		glm::vec2 deltaUV2 = uv3 - uv1;

		float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

		glm::vec3 tangent;
		tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
		tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
		tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
		tangent = glm::normalize(tangent);

		glm::vec3 bitangent;
		bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
		bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
		bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
		bitangent = glm::normalize(bitangent);

		for (int j = 0; j < 3; j++) {
			this->tangent.push_back(tangent);
			this->bitangent.push_back(bitangent);
		}
	}

	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);

	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);

	glBufferData(GL_ARRAY_BUFFER,
				this->vertex.size() * sizeof(glm::vec3) + this->normal.size() * sizeof(glm::vec3) + this->texture.size() * sizeof(glm::vec2) + this->tangent.size() * sizeof(glm::vec3) + this->bitangent.size() * sizeof(glm::vec3),
				0,
				GL_STATIC_DRAW);

	glBufferSubData(GL_ARRAY_BUFFER, 0, this->vertex.size() * sizeof(glm::vec3), this->vertex.data());
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

	glBufferSubData(GL_ARRAY_BUFFER,
					this->vertex.size() * sizeof(glm::vec3),
					this->normal.size() * sizeof(glm::vec3),
					this->normal.data());
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
						sizeof(glm::vec3),
						(void*)(this->vertex.size() * sizeof(glm::vec3)));

	glBufferSubData(GL_ARRAY_BUFFER,
					this->vertex.size() * sizeof(glm::vec3) + this->normal.size() * sizeof(glm::vec3),
					this->texture.size() * sizeof(glm::vec2),
					this->texture.data());
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
						sizeof(glm::vec2),
						(void*)(this->vertex.size() * sizeof(glm::vec3) + this->normal.size() * sizeof(glm::vec3)));

	glBufferSubData(GL_ARRAY_BUFFER,
					this->vertex.size() * sizeof(glm::vec3) + this->normal.size() * sizeof(glm::vec3) + this->texture.size() * sizeof(glm::vec2),
					this->tangent.size() * sizeof(glm::vec3),
					this->tangent.data());
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE,
						sizeof(glm::vec3),
						(void*)(this->vertex.size() * sizeof(glm::vec3) + this->normal.size() * sizeof(glm::vec3) + this->texture.size() * sizeof(glm::vec2)));

	glBufferSubData(GL_ARRAY_BUFFER,
					this->vertex.size() * sizeof(glm::vec3) + this->normal.size() * sizeof(glm::vec3) + this->texture.size() * sizeof(glm::vec2) + this->tangent.size() * sizeof(glm::vec3),
					this->bitangent.size() * sizeof(glm::vec3),
					this->bitangent.data());
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE,
						sizeof(glm::vec3),
						(void*)(this->vertex.size() * sizeof(glm::vec3) + this->normal.size() * sizeof(glm::vec3) + this->texture.size() * sizeof(glm::vec2) + this->tangent.size() * sizeof(glm::vec3)));
	
	glBindVertexArray(0);
}


void Object::modelInit() {
	this->model = glm::mat4(1);
	this->scaleMat = glm::mat4(1);
	this->rotateMat = glm::mat4(1);
	this->translateMat = glm::mat4(1);
}

void Object::scale(glm::vec3 size) {
	this->scaleMat = glm::scale(glm::mat4(1), size);
	this->model = this->translateMat * this->rotateMat * this->scaleMat;
}

void Object::rotate(float angle, glm::vec3 axis) {
	this->rotateMat = glm::rotate(glm::mat4(1), angle, axis);
	this->model = this->translateMat * this->rotateMat * this->scaleMat;
}

void Object::translate(glm::vec3 pos) {
	this->translateMat = glm::translate(glm::mat4(1), pos);
	this->model = this->translateMat * this->rotateMat * this->scaleMat;
}

glm::mat4& Object::getModel() {
	return this->model;
}

void Object::drawTriangles() {
	glBindVertexArray(this->VAO);
	glDrawArrays(GL_TRIANGLES, 0, this->vertex.size());
	glBindVertexArray(0);
}

void Object::drawLines() {
	glBindVertexArray(this->VAO);
	glDrawArrays(GL_LINES, 0, this->vertex.size());
	glBindVertexArray(0);
}

void Object::drawPoints() {
	glBindVertexArray(this->VAO);
	glDrawArrays(GL_POINTS, 0, this->vertex.size());
	glBindVertexArray(0);
}

void Object::erase() {
	if (this->VBO)
		glDeleteBuffers(1, &this->VBO);
	this->VBO = 0;

	if (this->VAO)
		glDeleteVertexArrays(1, &this->VAO);
	this->VAO = 0;
}