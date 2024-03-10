#ifndef IFRAMEBUFFER_HPP
#define IFRAMEBUFFER_HPP

#include "pch.hpp"

class iFramebuffer{

	protected:

		GLuint framebuffer;
		GLuint texture;
		GLuint renderbuffer;

		void setError();

	public:

		// iFramebuffer();
		// virtual ~iFramebuffer();
		// virtual iFramebuffer(const iFramebuffer& other);
		// virtual iFramebuffer& operator=(const iFramebuffer& other);
		void use();
		void unuse();
		GLuint getTexture();

};

#endif