#include "UniformBlockManager.hpp"

UniformBlockManager::UniformBlockManager(std::string const &name, std::map<std::string, Program>& programs) {
	for (std::map<std::string, Program>::iterator it = programs.begin(); it != programs.end(); it++) {
		GLuint id = glGetUniformBlockIndex(it->second.id(), name.c_str());
		this->uniformBlock.insert(std::make_pair(name, id));
		glUniformBlockBinding(it->second.id(), id, 0);
	}
	glGenBuffers(1, &this->uniformBlockObject);
}

UniformBlockManager::~UniformBlockManager() {}

// UniformBlockManager::UniformBlockManager(const UniformBlockManager& other) {}

// UniformBlockManager& UniformBlockManager::operator=(const UniformBlockManager& other) {
// 	if (this == &other)
// 		return *this;
// 	return *this;
// }

GLuint UniformBlockManager::getUniformBlockObject() {
	return this->uniformBlockObject;
}

void UniformBlockManager::erase() {
	glDeleteBuffers(1, &this->uniformBlockObject);
}

void UniformBlockManager::use() {
	glBindBuffer(GL_UNIFORM_BUFFER, this->uniformBlockObject);
}

void UniformBlockManager::unuse() {
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}