#ifndef PERSON_HPP
#define PERSON_HPP

#include "pch.hpp"

#include "Skeleton.hpp"
#include "Object.hpp"

class Person{

	private:
		Object point;
		Object line;
		GLuint VAO_body;
		GLuint VBO_cube;
		GLuint VBO_joints;

		std::vector<glm::mat4> joints;
		const std::vector<glm::vec3> cubeVertices = {
			{-0.5f, -0.5f, 0.5f},
			{0.5f, -0.5f, 0.5f},
			{0.5f,  0.5f, 0.5f},
			{-0.5f, -0.5f, 0.5f},
			{0.5f,  0.5f, 0.5f},
			{-0.5f,  0.5f, 0.5f},
			{-0.5f, -0.5f, -0.5f},
			{0.5f, -0.5f, -0.5f},
			{0.5f,  0.5f, -0.5f},
			{-0.5f, -0.5f, -0.5f},
			{0.5f,  0.5f, -0.5f},
			{-0.5f,  0.5f, -0.5f},
			{-0.5f, -0.5f, -0.5f},
			{-0.5f,  0.5f, -0.5f},
			{-0.5f,  0.5f, 0.5f},
			{-0.5f, -0.5f, -0.5f},
			{-0.5f,  0.5f, 0.5f},
			{-0.5f, -0.5f, 0.5f},
			{0.5f, -0.5f, -0.5f},
			{0.5f,  0.5f, -0.5f},
			{0.5f,  0.5f, 0.5f},
			{0.5f, -0.5f, -0.5f},
			{0.5f,  0.5f, 0.5f},
			{0.5f, -0.5f, 0.5f},
			{-0.5f,  0.5f, -0.5f},
			{0.5f,  0.5f, -0.5f},
			{0.5f,  0.5f, 0.5f},
			{-0.5f,  0.5f, -0.5f},
			{0.5f,  0.5f, 0.5f},
			{-0.5f,  0.5f, 0.5f},
			{-0.5f, -0.5f, -0.5f},
			{0.5f, -0.5f, -0.5f},
			{0.5f, -0.5f, 0.5f},
			{-0.5f, -0.5f, -0.5f},
			{0.5f, -0.5f, 0.5f},
			{-0.5f, -0.5f, 0.5f}
		};


	public:
		Person() = delete;
		Person(Skeleton& sk);
		~Person();
		void drawPoint();
		void drawLine();
		void drawBody();

};

#endif