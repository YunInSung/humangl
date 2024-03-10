#ifndef FRAMEBUFFER_HPP
#define FRAMEBUFFER_HPP

#include "pch.hpp"

#include "iFramebuffer.hpp"

class Framebuffer : public iFramebuffer {

	private:

		// GLuint framebuffer;
		// GLuint texture;
		// GLuint renderbuffer;

		// void setError();

	public:

		Framebuffer() = delete;
		Framebuffer(GLsizei width, GLsizei height, bool Alpha, int HDR);
		// ~Framebuffer();
		// Framebuffer(const Framebuffer& other);
		// Framebuffer& operator=(const Framebuffer& other);

		// void use();
		// GLuint getTexture();

};

#endif