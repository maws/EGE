#pragma once

#define _CRT_SECURE_NO_WARNINGS 1
#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>

#include <GL\gl3w.h>
#define GLFW_NO_GLU 1
#define GLFW_INCLUDE_GLCOREARB 1

#include <GLFW\glfw3.h>

#include <GL\glext.h>

namespace EGE
{
	class Application
	{
	public:
		Application();
		virtual ~Application();

		/// \brief Initialized OpenGL libraries and the window handle
		virtual void init();

		/// \brief Runs an application in an update/render loop
		/// \param[in] app The application that you wish to run
		void run(Application* app);

		/// \brief Called once before each update/render loop
		virtual void startup();

		/// \brief Called once after each update/render loop
		virtual void shutdown();

		/// \brief Render call
		virtual void render(double currentTime);

		/// \brief Resize callback
		virtual void onResize(int w, int h);

		/// \brief Key callback
		virtual void onKey(int key, int action);

		/// \brief Mouse button callback
		virtual void onMouseButton(int button, int action);

		/// \brief Mouse move callback
		virtual void onMouseMove(int x, int y);

		/// \brief Mouse wheel callback
		virtual void onMouseWheel(int pos);

		protected:

		static Application* app_;

		// Override glfw callbacks
		// These callbacks are set up in init()
		static void glfw_onResize(GLFWwindow* window, int w, int h)
		{
			app_->onResize(w, h);
		}
		static void glfw_onKey(GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			app_->onKey(key, action);
		}
		static void glfw_onMouseButton(GLFWwindow* window, int button, int action, int mods)
		{
			app_->onMouseButton(button, action);
		}
		static void glfw_onMouseMove(GLFWwindow* window, double x, double y)
		{
			app_->onMouseMove(static_cast<int>(x), static_cast<int>(y));
		}
		static void glfw_onMouseWheel(GLFWwindow* window, double xoffset, double yoffset)
		{
			app_->onMouseWheel(static_cast<int>(yoffset));
		}

		/// \brief Window info used when creating a glfw window
		struct WindowInfo
		{
			char title[128];
			int width;
			int height;
			int majorVersion;
			int minorVersion;
			int samples;
		};
		WindowInfo windowInfo_;
		GLFWwindow* window_;

		/// \brief Debug message callback
		/// \todo Multi-platform support
		virtual void onDebugMessage(GLenum source,
			GLenum type,
			GLuint id,
			GLenum severity,
			GLsizei length,
			const GLchar* message)
		{
#ifdef _WIN32
			OutputDebugStringA(message);
			OutputDebugStringA("\n");
#endif /* _WIN32 */
		}

		static void APIENTRY debug_callback(GLenum source,
			GLenum type,
			GLuint id,
			GLenum severity,
			GLsizei length,
			const GLchar* message,
			GLvoid* userParam)
		{
			reinterpret_cast<Application *>(userParam)->onDebugMessage(source, type, id, severity, length, message);
		}
	};
}