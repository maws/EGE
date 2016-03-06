#pragma once

#include <EGE\Application.hpp>

namespace EGE
{
	namespace ShaderUtil
	{
		GLuint loadShader(const char* fileName, GLenum shaderType);
	}
	
	class RenderProgram
	{
	public:
		RenderProgram();
		~RenderProgram();

		GLuint create(const GLuint * shaders, int count);
		const GLuint getProgram() const { return m_renderProgram; }

	private:
		GLuint m_renderProgram;

	};
}