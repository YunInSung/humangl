#include "iFramebuffer.hpp"

// iFramebuffer::iFramebuffer() {}

// iFramebuffer::~iFramebuffer() {}

// iFramebuffer::iFramebuffer(const iFramebuffer& other) {}

// iFramebuffer& iFramebuffer::operator=(const iFramebuffer& other) {
// 	if (this == &other)
// 		return *this;
// 	return *this;
// }

void iFramebuffer::setError() {
	glDeleteFramebuffers(1, &this->framebuffer);
	glDeleteTextures(1, &this->texture);
	glDeleteRenderbuffers(1, &this->renderbuffer);
	this->framebuffer = 0;
	this->texture = 0;
	this->renderbuffer = 0;
}

void iFramebuffer::use() {
	glBindFramebuffer(GL_FRAMEBUFFER, this->framebuffer);
}

void iFramebuffer::unuse() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLuint iFramebuffer::getTexture() {
	return this->texture;
}