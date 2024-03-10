#include "Framebuffer.hpp"

Framebuffer::Framebuffer(GLsizei width, GLsizei height, bool Alpha = false, int HDR = 0) {
	glGenFramebuffers(1, &this->framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, this->framebuffer);

	glGenTextures(1, &this->texture);

	glBindTexture(GL_TEXTURE_2D, this->texture);

	if (Alpha) {
		if (HDR == 16)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
		else if (HDR == 32)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
		else
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	} else {
		if (HDR == 16)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
		else if (HDR == 32)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
		else
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->texture, 0);

	glGenRenderbuffers(1, &this->renderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, this->renderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, this->renderbuffer);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		this->setError();
		return ;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

// Framebuffer::~Framebuffer() {}

// Framebuffer::Framebuffer(const Framebuffer& other) {}

// Framebuffer& Framebuffer::operator=(const Framebuffer& other) {
// 	if (this == &other)
// 		return *this;
// 	return *this;
// }

// void Framebuffer::setError() {
// 	glDeleteFramebuffers(1, &this->framebuffer);
// 	glDeleteTextures(1, &this->texture);
// 	glDeleteRenderbuffers(1, &this->renderbuffer);
// 	this->framebuffer = 0;
// 	this->texture = 0;
// 	this->renderbuffer = 0;
// }

// void Framebuffer::use() {
// 	glBindFramebuffer(GL_FRAMEBUFFER, this->framebuffer);
// }

// GLuint Framebuffer::getTexture() {
// 	return this->texture;
// }