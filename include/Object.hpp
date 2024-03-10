#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "pch.hpp"

class Object{

	private:
		GLuint VAO;
		GLuint VBO;

		std::vector<glm::vec3> vertex;
		std::vector<glm::vec3> normal;
		std::vector<glm::vec2> texture;
		
		std::vector<glm::vec3> tangent;
		std::vector<glm::vec3> bitangent;

		glm::mat4 model;

		glm::mat4 scaleMat;
		glm::mat4 rotateMat;
		glm::mat4 translateMat;


	public:

		Object();
		~Object();
		Object(const Object& other);
		Object& operator=(const Object& other);


		void setVertex(const std::vector<glm::vec3>& v);
		void setNormal(const std::vector<glm::vec3>& n);
		void setTexture(const std::vector<glm::vec2>& t);
		void setting();
		void normalMappingSetting();

		glm::mat4& getModel();
		void scale(glm::vec3 size);
		void rotate(float angle, glm::vec3 axis);
		void translate(glm::vec3 pos);
		void modelInit();

		void drawTriangles();
		void drawLines();
		void drawPoints();

		void erase();
};

#endif