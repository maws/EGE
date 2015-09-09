#pragma once

#include <EGE\Application.hpp>

namespace EGE
{
	/// \brief Creates and compiles a shader from file name
	/// \param[in] fileName Relative file name
	/// \param[in] shaderType OpenGL shader type used when calling glCreateShader
	GLuint loadShader(const char* fileName, GLenum shaderType);

	class RenderProgram
	{
	public:
		RenderProgram();
		~RenderProgram();

		/// \brief Creates an OpenGL render program from specified shaders and links them
		/// \param[in] shaders An array of OpenGL shaders
		/// \param[in] count The length of the array passed as shaders
		GLuint create(const GLuint * shaders, int count);

		/// \return GLuint OpenGL render program
		GLuint getProgram(){ return program_; }

	private:
		GLuint program_;

	};
}