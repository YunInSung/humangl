#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "pch.hpp"

class Camera {

	private:
		glm::mat4 Projection;

		float zFar;
		float zNear;

		glm::mat4 View;
		glm::vec3 Position;
		glm::vec3 LookAt;
		glm::vec3 UP;
		glm::vec3 UPfromDirection;
		glm::vec3 right;

		glm::vec3 direction;

		int width;
		int height;

		float horizontalAngle;
		float verticalAngle;
		float FoV;
		float speed;
		float mouseSpeed;

		bool firstMouse;

		bool frontState;
		bool backState;
		bool leftState;
		bool rightState;
		bool upState;
		bool downState;

		void setMousePos(double xpos, double ypos);

	public:
		Camera() = delete;
		Camera(int width, int height);

		glm::mat4 getView();
		glm::mat4 getProjection();

		void setFront(bool state);
		void setBack(bool state);
		void setLeft(bool state);
		void setRight(bool state);
		void setUp(bool state);
		void setDown(bool state);
		void changeFov(int num);
		void resetFov();
		float getFov();
		glm::vec3& getDirection();
		glm::vec3& getUp();
		glm::vec3& getUpFromDirection();
		glm::vec3& getRight();
		float getWidth();
		float getHeight();
		float getZFar();
		float getZNear();

		glm::vec3 getPosition();
		float getHorizonAngle();
		float getVerticalAngle();

		void render(double deltaTime, double xpos, double ypos);
};

#endif