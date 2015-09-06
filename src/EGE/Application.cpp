#include <EGE\Application.hpp>
#include <stdlib.h>
#include <string>
#include <vmath.h>

#include <GL\glext.h>
GL3WglProc sb6GetProcAddress(const char * funcname)
{
	return gl3wGetProcAddress(funcname);
}

int sb6IsExtensionSupported(const char * extname)
{
	GLint numExtensions;
	GLint i;

	glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions);

	for (i = 0; i < numExtensions; i++)
	{
		const GLubyte * e = glGetStringi(GL_EXTENSIONS, i);
		if (!strcmp((const char *)e, extname))
		{
			return 1;
		}
	}

	return 0;
}

namespace EGE
{
	Application* Application::app_ = nullptr;

	Application::Application()
		: window_(nullptr)
	{

	}

	Application::~Application()
	{
		
	}

	void Application::init()
	{
		// Init glfw
		int res = glfwInit();

		// Set window info
		const char* title = "EGEFramework v0.1";
		windowInfo_.majorVersion = 4;
		windowInfo_.minorVersion = 3;
		windowInfo_.samples = 0;
		memcpy(&windowInfo_.title, title, sizeof("EGEFramework v0.1"));
		windowInfo_.height = 600;
		windowInfo_.width = 800;

		// Set window hints
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, windowInfo_.majorVersion);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, windowInfo_.minorVersion);
#ifdef _DEBUG
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		glfwWindowHint(GLFW_SAMPLES, windowInfo_.samples);
		glfwWindowHint(GLFW_STEREO, GL_FALSE);

		window_ = glfwCreateWindow(windowInfo_.width, windowInfo_.height, windowInfo_.title, nullptr, nullptr);
		glfwMakeContextCurrent(window_);

		// Override callbacks
		glfwSetWindowSizeCallback(window_, glfw_onResize);
		glfwSetKeyCallback(window_, glfw_onKey);
		glfwSetMouseButtonCallback(window_, glfw_onMouseButton);
		glfwSetCursorPosCallback(window_, glfw_onMouseMove);
		glfwSetScrollCallback(window_, glfw_onMouseWheel);
		glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		gl3wInit();

#ifdef _DEBUG
		fprintf(stderr, "VENDOR: %s\n", (char *)glGetString(GL_VENDOR));
		fprintf(stderr, "VERSION: %s\n", (char *)glGetString(GL_VERSION));
		fprintf(stderr, "RENDERER: %s\n", (char *)glGetString(GL_RENDERER));

		if (gl3wIsSupported(4, 3))
		{
			glDebugMessageCallback((GLDEBUGPROC)debug_callback, this);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		}
		else if (sb6IsExtensionSupported("GL_ARB_debug_output"))
		{
			glDebugMessageCallbackARB((GLDEBUGPROC)debug_callback, this);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
		}
#endif
	}

	void Application::run(Application* app)
	{
		app_ = app;

		init();
		startup();

		bool running = true;
		while (running)
		{
			render(glfwGetTime());
			glfwSwapBuffers(window_);
			glfwPollEvents();
			running &= (glfwGetKey(window_, GLFW_KEY_ESCAPE) == GLFW_RELEASE);
			running &= (glfwWindowShouldClose(window_) != GL_TRUE);
		}

		shutdown();
		glfwDestroyWindow(window_);
		glfwTerminate();
	}

	void Application::onResize(int w, int h)
	{
		windowInfo_.width = w;
		windowInfo_.height = h;
	}

	void Application::onKey(int key, int action)
	{

	}

	void Application::onMouseButton(int button, int action)
	{

	}

	void Application::onMouseMove(int x, int y)
	{

	}

	void Application::onMouseWheel(int pos)
	{

	}

	void Application::startup()
	{

	}

	void Application::shutdown()
	{

	}

	void Application::render(double currentTime)
	{

	}

}