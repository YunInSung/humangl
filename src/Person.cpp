#include "Person.hpp"

Person::Person(Skeleton& sk) {
	this->point.setVertex(sk.getPoints());
	this->point.setting();

	this->line.setVertex(sk.getLines());
	this->line.setting();

	this->joints = sk.getcube();

	glGenVertexArrays(1, &this->VAO_body);
	glGenBuffers(1, &this->VBO_cube);
	glGenBuffers(1, &this->VBO_joints);

	glBindVertexArray(this->VAO_body);

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO_cube);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * this->cubeVertices.size(), this->cubeVertices.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO_joints);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * this->joints.size(), this->joints.data(), GL_STREAM_DRAW);

	size_t vec4Size = sizeof(glm::vec4);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(0 * vec4Size));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(1 * vec4Size));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));

	glVertexAttribDivisor(1, 1);
	glVertexAttribDivisor(2, 1);
	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);

	glBindVertexArray(0);
}

Person::~Person() {
	glDeleteBuffers(1, &this->VBO_cube);
	glDeleteBuffers(1, &this->VBO_joints);
	glDeleteVertexArrays(1, &this->VAO_body);
}

void Person::drawPoint() {
	this->point.drawPoints();
}

void Person::drawLine() {
	this->line.drawLines();
}

void Person::drawBody() {
	glBindVertexArray(this->VAO_body);
	glDrawArraysInstanced(GL_TRIANGLES, 0, this->cubeVertices.size(), this->joints.size());
	glBindVertexArray(0);
}