#include "App.hpp"

App::App(): camera(500, 500), is_error(false) {
	this->width = 500;
	this->height = 500;
	this->init("tmp");

	this->lastTime = glfwGetTime();

	this->frame = 0;
	this->frameTime = glfwGetTime();

	this->cameraFirst = true;

	glfwGetFramebufferSize(this->window, &this->framebufferWidth, &this->framebufferHeight);
}

App::App(std::string const &title, int width, int height): camera(width, height), is_error(false) {
	this->width = width;
	this->height = height;
	this->init(title);

	this->lastTime = glfwGetTime();

	this->frame = 0;
	this->frameTime = glfwGetTime();

	this->cameraFirst = true;

	glfwGetFramebufferSize(this->window, &this->framebufferWidth, &this->framebufferHeight);
}

App::~App() {
	for (std::map<std::string, GLuint>::iterator it = this->texture.begin(); it != this->texture.end(); it++)
		glDeleteTextures(1, &it->second);
	glfwDestroyWindow(this->window);
	glfwTerminate();
}

void App::setError(std::string const &msg) {
	std::cout << "\033[32mAPP ERROR : \033[35m" << msg << "\033[0m" << std::endl;
	this->is_error = true;
}

bool App::isError() {
	return this->is_error;
}


void App::init(std::string const &title) {
	glfwSetErrorCallback(errorCallback);

	if (!glfwInit())
		return this->setError("Fail to init glfw");

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	#endif
	this->window = glfwCreateWindow(this->width, this->height, title.c_str(), NULL, NULL);
	if (!this->window) {
		glfwTerminate();
		return this->setError("Fail to make window");
	}

	glfwSetWindowUserPointer(this->window, this);

	glfwMakeContextCurrent(this->window);
	glfwSetKeyCallback(window, keyCallback);
    glfwSetMouseButtonCallback(window, mouseCallback);
	glfwSetScrollCallback(window, scrollCallback); 

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        return this->setError("Failed to initialize OpenGL context");
    }
}

void App::setting() {
	glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void App::setProgram(std::string const &name, std::string const &vs, std::string const &fs) {
	Program pro;
	pro.loadVertex(vs);
	pro.loadFragment(fs);
	pro.loadProgram();
	if (!pro.is_set())
		return this->setError("Fail to compile Program");
	else {
		this->programs.insert(std::make_pair(name, pro));
		pro.use();
	}
}

void App::setProgram(std::string const &name, std::string const &vs, std::string const &gs, std::string const &fs) {
	Program pro;
	pro.loadVertex(vs);
	pro.loadGeometry(gs);
	pro.loadFragment(fs);
	pro.loadProgram();
	if (!pro.is_set())
		return this->setError("Fail to compile Program");
	else {
		this->programs.insert(std::make_pair(name, pro));
		pro.use();
	}
}

void App::useProgram(std::string const &name) {
	if (this->programs.count(name) == 0)
		return;
	this->programs.find(name)->second.use();
}

void	App::renderCamera(double deltaTime) {
	double xpos, ypos;
	glfwGetCursorPos(this->window, &xpos, &ypos);
	glfwSetCursorPos(window, this->width/2, this->height/2);
	if (this->cameraFirst)
		this->cameraFirst = false;
	else
		this->camera.render(deltaTime, xpos, ypos);
}

void App::setFrameTitle() {
	if (this->frame == 0 || glfwGetTime() - this->frameTime > 0.1) {
		std::stringstream ss;
		ss << this->lastTime << " : ";
		ss << " [" << this->frame / (glfwGetTime() - this->frameTime) << " FPS]";
		glfwSetWindowTitle(this->window, ss.str().c_str());
		float fps = roundf((this->frame / (glfwGetTime() - this->frameTime)) * 100) / 100.f;
		this->frame = 0;
		this->frameTime = glfwGetTime();
	}
	this->frame++;
}

// for callback
void	App::setFront(bool status) {
	this->camera.setFront(status);
}
void	App::setBack(bool status) {
	this->camera.setBack(status);
}
void	App::setLeft(bool status) {
	this->camera.setLeft(status);
}
void	App::setRight(bool status) {
	this->camera.setRight(status);
}
void	App::setUp(bool status) {
	this->camera.setUp(status);
}
void	App::setDown(bool status) {
	this->camera.setDown(status);
}
void	App::changeFov(int num) {
	this->camera.changeFov(num);
}
void	App::resetFov() {
	this->camera.resetFov();
}

void App::setBool(std::string const &programName, std::string const &name, bool value) {
	if (this->programs.count(programName) == 0)
		return;
	this->programs.find(programName)->second.setBool(name, value);
}
void App::setInt(std::string const &programName, std::string const &name, int value) {
	if (this->programs.count(programName) == 0)
		return;
	this->programs.find(programName)->second.setInt(name, value);
}
void App::setFloat(std::string const &programName, std::string const &name, float value) {
	if (this->programs.count(programName) == 0)
		return;
	this->programs.find(programName)->second.setFloat(name, value);
}
void App::setVec2(std::string const &programName, std::string const &name, glm::vec2 value) {
	if (this->programs.count(programName) == 0)
		return;
	this->programs.find(programName)->second.setVec2(name, value);
}
void App::setVec2(std::string const &programName, std::string const &name, float x, float y) {
	if (this->programs.count(programName) == 0)
		return;
	this->programs.find(programName)->second.setVec2(name, x, y);
}
void App::setVec3(std::string const &programName, std::string const &name, glm::vec3 value) {
	if (this->programs.count(programName) == 0)
		return;
	this->programs.find(programName)->second.setVec3(name, value);
}
void App::setVec3(std::string const &programName, std::string const &name, float x, float y, float z) {
	if (this->programs.count(programName) == 0)
		return;
	this->programs.find(programName)->second.setVec3(name, x, y, z);
}
void App::setVec4(std::string const &programName, std::string const &name, glm::vec4 value) {
	if (this->programs.count(programName) == 0)
		return;
	this->programs.find(programName)->second.setVec4(name, value);
}
void App::setVec4(std::string const &programName, std::string const &name, float x, float y, float z, float w) {
	if (this->programs.count(programName) == 0)
		return;
	this->programs.find(programName)->second.setVec4(name, x, y, z, w);
}
void App::setMat3(std::string const &programName, std::string const &name, glm::mat3 &mat) {
	if (this->programs.count(programName) == 0)
		return;
	this->programs.find(programName)->second.setMat3(name, mat);
}
void App::setMat4(std::string const &programName, std::string const &name, glm::mat4 &mat) {
	if (this->programs.count(programName) == 0)
		return;
	this->programs.find(programName)->second.setMat4(name, mat);
}

// for callback
static void	keyCallback(GLFWwindow *window, int key, int scancode, int action, int mode) {
	App *app = static_cast<App*>(glfwGetWindowUserPointer(window));
	if (!app)
		return ;

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

	if (key == GLFW_KEY_W && action == GLFW_PRESS)
		app->setFront(true);
	else if (key == GLFW_KEY_W && action == GLFW_RELEASE)
		app->setFront(false);
		
	if (key == GLFW_KEY_S && action == GLFW_PRESS)
		app->setBack(true);
	else if (key == GLFW_KEY_S && action == GLFW_RELEASE)
		app->setBack(false);

	if (key == GLFW_KEY_A && action == GLFW_PRESS)
		app->setLeft(true);
	else if (key == GLFW_KEY_A && action == GLFW_RELEASE)
		app->setLeft(false);

	if (key == GLFW_KEY_D && action == GLFW_PRESS)
		app->setRight(true);
	else if (key == GLFW_KEY_D && action == GLFW_RELEASE)
		app->setRight(false);

	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
		app->setUp(true);
	else if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE)
		app->setUp(false);

	if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_PRESS)
		app->setDown(true);
	else if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_RELEASE)
		app->setDown(false);
}


static void	mouseCallback(GLFWwindow* window, int button, int action, int mods) {
	App *app = static_cast<App*>(glfwGetWindowUserPointer(window));
	if (!app)
		return ;

	if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS)
		app->setUp(true);
	else if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_RELEASE)
		app->setUp(false);

	if (button == GLFW_MOUSE_BUTTON_2 && action == GLFW_PRESS)
		app->setDown(true);
	else if (button == GLFW_MOUSE_BUTTON_2 && action == GLFW_RELEASE)
		app->setDown(false);

	if (button == GLFW_MOUSE_BUTTON_3 && action == GLFW_PRESS)
		app->resetFov();
}

static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	App *app = static_cast<App*>(glfwGetWindowUserPointer(window));
	if (!app)
		return ;

	app->changeFov(static_cast<int>(-yoffset));
}

static void errorCallback(int error, const char *des) {
	std::cout << des << std::endl;
}

static bool endsWith(std::string const &str, std::string const &suffix) {
    if (str.length() < suffix.length()) {
        return false;
    }
    return str.compare(str.length() - suffix.length(), suffix.length(), suffix) == 0;
}

static void err() {
	GLenum err = glGetError();
	switch (err) {
		case GL_NO_ERROR:
			std::cout << "GL_NO_ERROR" << std::endl;
			break;
		case GL_INVALID_ENUM:
			std::cout << "GL_INVALID_ENUM" << std::endl;
			break;
		case GL_INVALID_VALUE:
			std::cout << "GL_INVALID_VALUE" << std::endl;
			break;
		case GL_INVALID_OPERATION:
			std::cout << "GL_INVALID_OPERATION" << std::endl;
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			std::cout << "GL_INVALID_FRAMEBUFFER_OPERATION" << std::endl;
			break;
		case GL_OUT_OF_MEMORY:
			std::cout << "GL_OUT_OF_MEMORY" << std::endl;
			break;
		case GL_STACK_UNDERFLOW:
			std::cout << "GL_STACK_UNDERFLOW" << std::endl;
			break;
		case GL_STACK_OVERFLOW:
			std::cout << "GL_STACK_OVERFLOW" << std::endl;
			break;
		default:
			std::cout << "no err" << std::endl;
			break;
	}
}