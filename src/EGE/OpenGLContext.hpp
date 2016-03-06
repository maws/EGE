#pragma once

#define _CRT_SECURE_NO_WARNINGS 1
#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>

#include <GL\gl3w.h>
#define GLFW_NO_GLU 1
#define GLFW_INCLUDE_GLCOREARB 1

#include <GLFW\glfw3.h>
#include <GL\glext.h>

#include <vector>
#include <functional>

namespace EGE
{
	class RenderProgram;
	class OpenGLContext
	{
	public:
		OpenGLContext();

		bool init();
		void destroy();
		void swapBuffers();
		void pollEvents();
		double getTime();

		void getWindowSize(int* w, int* h)
		{
			glfwGetWindowSize(m_window, w, h);
		}

		static void APIENTRY debugCallback(GLenum src, GLenum type, GLuint id, GLenum priority, GLsizei length, const GLchar* msg, GLvoid* userParams);
		
		GLuint m_vertexArray;
		GLFWwindow* m_window;
		RenderProgram* m_defaultShader;
	};
}