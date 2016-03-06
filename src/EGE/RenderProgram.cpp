#include <EGE\RenderProgram.hpp>

#include <stdio.h>

namespace EGE
{
	RenderProgram::RenderProgram()
	{

	}

	RenderProgram::~RenderProgram()
	{
		glDeleteProgram(m_renderProgram);
	}

	GLuint RenderProgram::create(const GLuint * shaders, int count)
	{
		int i;
		m_renderProgram = glCreateProgram();
		for (i = 0; i < count; ++i)
		{
			glAttachShader(m_renderProgram, shaders[i]);
		}
		glLinkProgram(m_renderProgram);

#ifdef _DEBUG
		GLint status;
		glGetProgramiv(m_renderProgram, GL_LINK_STATUS, &status);
		if (!status)
		{
			char buffer[4096];
			glGetProgramInfoLog(m_renderProgram, 4096, NULL, buffer);
			OutputDebugStringA(buffer);
			OutputDebugStringA("\n");
			glDeleteProgram(m_renderProgram);
			return 0;
		}
#endif
		for (i = 0; i < count; ++i)
		{
			glDeleteShader(shaders[i]);
		}

		return m_renderProgram;
	}

	namespace ShaderUtil
	{
		GLuint loadShader(const char* fileName, GLenum shaderType)
		{
			size_t fileSize;
			char* data;
			FILE* file;
			file = fopen(fileName, "rb");
			fseek(file, 0, SEEK_END);
			fileSize = ftell(file);
			fseek(file, 0, SEEK_SET);
			data = new char[fileSize + 1];
			fread(data, 1, fileSize, file);
			data[fileSize] = 0;
			fclose(file);

			GLuint shader = glCreateShader(shaderType);
			glShaderSource(shader, 1, &data, nullptr);
			delete[] data; // OpenGL has ownership
			glCompileShader(shader);

#ifdef _DEBUG
			char buffer[4096];
			glGetShaderInfoLog(shader, 4096, NULL, buffer);
			OutputDebugStringA(fileName);
			OutputDebugStringA(":");
			OutputDebugStringA(buffer);
			OutputDebugStringA("\n");
#endif
			return shader;
		}
	}
}