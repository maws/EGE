#include <EGE\Shader.hpp>

#include <stdio.h>

namespace EGE
{
	RenderProgram::RenderProgram()
	{

	}

	RenderProgram::~RenderProgram()
	{

	}

	GLuint RenderProgram::create(const GLuint * shaders, int count)
	{
		int i;

		// Create and link program
		program_ = glCreateProgram();
		for (i = 0; i < count; ++i)
		{
			glAttachShader(program_, shaders[i]);
		}
		glLinkProgram(program_);

		// Check errors
		// TODO: Disable in release builds
		GLint status;
		glGetProgramiv(program_, GL_LINK_STATUS, &status);
		if (!status)
		{
			char buffer[4096];
			glGetProgramInfoLog(program_, 4096, NULL, buffer);
			OutputDebugStringA(buffer);
			OutputDebugStringA("\n");
			glDeleteProgram(program_);
			return 0;
		}

		for (i = 0; i < count; ++i)
		{
			glDeleteShader(shaders[i]);
		}

		return program_;
	}

	GLuint loadShader(const char* fileName, GLenum shaderType)
	{
		// Read from file
		size_t fileSize;
		char* data;
		FILE* file;
		file = fopen(fileName, "rb");
		fseek(file, 0, SEEK_END);
		fileSize = ftell(file);
		fseek(file, 0, SEEK_SET);
		data = new char[fileSize + 1];
		fread(data, 1, fileSize, file);
		data[fileSize] = 0; // Zero terminate
		fclose(file);

		GLuint shader = glCreateShader(shaderType);
		glShaderSource(shader, 1, &data, nullptr);
		delete[] data; // OpenGL has ownership
		glCompileShader(shader);

		// Check errors
		// TODO: Disable in release builds
		char buffer[4096];
		glGetShaderInfoLog(shader, 4096, NULL, buffer);
		OutputDebugStringA(fileName);
		OutputDebugStringA(":");
		OutputDebugStringA(buffer);
		OutputDebugStringA("\n");

		return shader;
	}
}