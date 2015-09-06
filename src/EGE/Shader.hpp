#pragma once

#include <EGE\Application.hpp>

namespace EGE
{
	GLuint loadShader(const char* fileName, GLenum shaderType);

	class RenderProgram
	{
	public:
		RenderProgram();
		~RenderProgram();

		GLuint create(const GLuint * shaders, int count);
		GLuint getProgram(){ return program_; }

	private:
		GLuint program_;

	};
}