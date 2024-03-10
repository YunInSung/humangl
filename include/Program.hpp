#ifndef SHADER_HPP
#define SHADER_HPP

#include "pch.hpp"

class Program {
	private:
		GLuint shaderId[6];
		GLuint programId;

		void clearShader();
		GLuint loadShader(GLenum type, std::string const &filename);

	public:
		Program();
		~Program();

		void erase();

		void loadVertex(std::string const &filename);
		void loadTessControl(std::string const &filename);
		void loadTessEvaluation(std::string const &filename);
		void loadGeometry(std::string const &filename);
		void loadFragment(std::string const &filename);
		void loadCompute(std::string const &filename);
		bool loadProgram();

		GLuint id();

		bool is_set();
		void use();

		void setBool(std::string const &name, bool value);
		void setInt(std::string const &name, int value);
		void setFloat(std::string const &name, float value);
		void setVec2(std::string const &name, glm::vec2 value);
		void setVec2(std::string const &name, float x, float y);
		void setVec3(std::string const &name, glm::vec3 value);
		void setVec3(std::string const &name, float x, float y, float z);
		void setVec4(std::string const &name, glm::vec4 value);
		void setVec4(std::string const &name, float x, float y, float z, float w);
		void setMat3(std::string const &name, glm::mat3 &mat);
		void setMat4(std::string const &name, glm::mat4 &mat);
};

#endif
