#pragma once
#include <GL/glew.h>

namespace GLUtil {
	class Shader {
		GLuint shaderid;
		bool compiled;
	public:
		Shader();
		Shader(Shader&) = delete;
		void compile(char const* const vertexSource, char const* const fragmentSource);
		~Shader();
		GLuint id();
		bool isCompiled();
		void select();

		//All of the GL BULLSHIT
		GLuint getAttrib(const char* name);
		GLuint getUniform(const char* name);
	};
}