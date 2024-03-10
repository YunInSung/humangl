#include "Camera.hpp"

Camera::Camera(int width, int height) {
	this->FoV = 45.0f;
	this->width = width;
	this->height = height;
	this->zFar = 1000.f;
	this->zNear = 0.1f;
	this->Projection = glm::perspective(glm::radians(this->FoV), width / static_cast<float>(height), zNear, zFar);
	this->Position = glm::vec3(0, 0, 0);
	this->UP = glm::vec3(0, 1, 0);
	this->horizontalAngle = 3.14f;
	this->verticalAngle = 0;
	this->speed = 10.0f;
	this->mouseSpeed = 0.002f;

	this->firstMouse = true;

	this->frontState = false;
	this->backState = false;
	this->leftState = false;
	this->rightState = false;
	this->upState = false;
	this->downState = false;
}

glm::mat4 Camera::getView() {
	return this->View;
}

glm::mat4 Camera::getProjection() {
	return this->Projection;
}

void Camera::setFront(bool state) {
	this->frontState = state;
}

void Camera::setBack(bool state) {
	this->backState = state;
}

void Camera::setLeft(bool state) {
	this->leftState = state;
}

void Camera::setRight(bool state) {
	this->rightState = state;
}

void Camera::setUp(bool state) {
	this->upState = state;
}

void Camera::setDown(bool state) {
	this->downState = state;
}

void Camera::changeFov(int num) {
	this->FoV += num;
	if (this->FoV > 120)
		this->FoV = 120;
	else if (this->FoV < 15)
		this->FoV = 15;
}
void Camera::resetFov() {
	this->FoV = 45;
}

float Camera::getFov() {
	return this->FoV;
}

glm::vec3& Camera::getDirection() {
	return this->direction;
}

glm::vec3 Camera::getPosition() {
	return this->Position;
}

float Camera::getWidth() {
	return this->width;
}
float Camera::getHeight() {
	return this->height;
}

float Camera::getZFar() {
	return this->zFar;
}
float Camera::getZNear() {
	return this->zNear;
}
glm::vec3& Camera::getUp() {
	return this->UP;
}
glm::vec3& Camera::getUpFromDirection() {
	return this->UPfromDirection;
}
glm::vec3& Camera::getRight() {
	return this->right;
}

float Camera::getHorizonAngle() {
	return this->horizontalAngle;
}
float Camera::getVerticalAngle() {
	return this->verticalAngle;
}

void Camera::setMousePos(double xpos, double ypos) {
	if (this->firstMouse) {
		this->firstMouse = false;
		return;
	}
	this->horizontalAngle += mouseSpeed * float(this->width/2 - xpos);
	if (this->horizontalAngle > glm::radians(360.f) || this->horizontalAngle < glm::radians(360.f) * -1)
		this->horizontalAngle = 0;
	this->verticalAngle += mouseSpeed * float(this->height/2 - ypos);
	if (this->verticalAngle >= glm::radians(89.f))
		this->verticalAngle = glm::radians(89.f);
	else if (this->verticalAngle <= glm::radians(89.f) * -1)
		this->verticalAngle = glm::radians(89.f) * -1;
}

void Camera::render(double deltaTime, double xpos, double ypos) {
	this->Projection = glm::perspective(glm::radians(this->FoV), width / static_cast<float>(height), zNear, zFar);

	this->setMousePos(xpos, ypos);

	// 카메라가 보고있는 방향
	this->direction = glm::vec3(
		cos(verticalAngle) * sin(horizontalAngle),
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	);

	// right vector
	this->right = glm::vec3(
		sin(horizontalAngle - glm::radians(90.f)), 
		0,
		cos(horizontalAngle - glm::radians(90.f))
	);

	// 카메라가 보고있는 방향에서 y축을 무시한 것
	glm::vec3 front(
		sin(horizontalAngle),
		0,
		cos(horizontalAngle)
	);
	
	this->UP = glm::cross(this->right, front);
	this->UPfromDirection = glm::cross(this->right, this->direction);

	if (this->frontState)
		this->Position += front * static_cast<float>(deltaTime) * speed;
	if (this->backState)
		this->Position -= front * static_cast<float>(deltaTime) * speed;
	if (this->rightState)
		this->Position += this->right * static_cast<float>(deltaTime) * speed;
	if (this->leftState)
		this->Position -= this->right * static_cast<float>(deltaTime) * speed;
	if (this->upState)
		this->Position += this->UP * static_cast<float>(deltaTime) * speed;
	if (this->downState)
		this->Position -= this->UP * static_cast<float>(deltaTime) * speed;

	glm::vec3 center = this->Position + this->direction;

	this->View = glm::lookAt(
		this->Position,
		center,
		this->UP
	);
}