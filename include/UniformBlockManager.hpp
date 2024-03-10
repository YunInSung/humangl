#ifndef UNIFORMBLOCKMANAGER_HPP
#define UNIFORMBLOCKMANAGER_HPP

#include "pch.hpp"

#include "Program.hpp"

class UniformBlockManager{

	private:

		GLuint uniformBlockObject;
		std::map<std::string, GLuint> uniformBlock;

	public:

		UniformBlockManager() = delete;
		UniformBlockManager(std::string const &name, std::map<std::string, Program>& programs);
		~UniformBlockManager();
		// UniformBlockManager(const UniformBlockManager& other);
		// UniformBlockManager& operator=(const UniformBlockManager& other);

		GLuint getUniformBlockObject();
		void use();
		void unuse();
		void erase();

};

#endif