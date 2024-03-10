#ifndef APP_HPP
#define APP_HPP

#include "pch.hpp"

#include "Camera.hpp"
#include "Program.hpp"
#include "Object.hpp"
#include "UniformBlockManager.hpp"
#include "Framebuffer.hpp"

#include "Skeleton.hpp"
#include "Person.hpp"


#define FRAME_FIX 60

class App{

	private:
		GLFWwindow			*window;
		int					width;
		int					height;

		int					framebufferWidth;
		int					framebufferHeight;

		Camera camera;
		bool cameraFirst;

		std::map<std::string, Program>	programs;
		std::map<std::string, GLuint>	texture;

		double lastTime;

		bool is_error;

		int frame;
		double frameTime;

		double frameFix;

		void	setError(std::string const &msg);

		void	setFrameTitle();

		void	useProgram(std::string const &name);
		void	renderCamera(double deltaTime);

		void setBool(std::string const &programName, std::string const &name, bool value);
		void setInt(std::string const &programName, std::string const &name, int value);
		void setFloat(std::string const &programName, std::string const &name, float value);
		void setVec2(std::string const &programName, std::string const &name, glm::vec2 value);
		void setVec2(std::string const &programName, std::string const &name, float x, float y);
		void setVec3(std::string const &programName, std::string const &name, glm::vec3 value);
		void setVec3(std::string const &programName, std::string const &name, float x, float y, float z);
		void setVec4(std::string const &programName, std::string const &name, glm::vec4 value);
		void setVec4(std::string const &programName, std::string const &name, float x, float y, float z, float w);
		void setMat3(std::string const &programName, std::string const &name, glm::mat3 &mat);
		void setMat4(std::string const &programName, std::string const &name, glm::mat4 &mat);

	public:
		App();
		App(std::string const &title, int width, int height);
		~App();

		bool	isError();

		void	init(std::string const &title);
		void	setting();

		void	setProgram(std::string const &name, std::string const &vs, std::string const &fs);
		void	setProgram(std::string const &name, std::string const &vs, std::string const &gs, std::string const &fs);
		void	addTexture(std::string const &name, std::string const &filename);
		void	addTexture(std::string const &name, std::vector<std::string> &faces);

		void	loop();

		// for callback
		void	setFront(bool status);
		void	setBack(bool status);
		void	setLeft(bool status);
		void	setRight(bool status);
		void	setUp(bool status);
		void	setDown(bool status);
		void	changeFov(int num);
		void	resetFov();
		// for callback
};


static void	keyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
static void	mouseCallback(GLFWwindow* window, int button, int action, int mods);
static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
static void errorCallback(int error, const char *des);
static bool endsWith(std::string const &str, std::string const &suffix);
static void err();

#endif