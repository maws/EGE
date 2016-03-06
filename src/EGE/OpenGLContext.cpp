#include <EGE/OpenGLContext.hpp>

#include <EGE/RenderProgram.hpp>
#include <EGE/Input.h>

GL3WglProc GetProcAddress(const char * funcname)
{
	return gl3wGetProcAddress(funcname);
}

bool IsExtensionSupported(const char * extname)
{
	GLint numExtensions;
	glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions);

	for (GLint i = 0; i < numExtensions; i++)
	{
		const GLubyte * e = glGetStringi(GL_EXTENSIONS, i);
		if (!strcmp((const char *)e, extname))
		{
			return true;
		}
	}

	return false;
}

namespace EGE
{
	OpenGLContext::OpenGLContext()
	{

	}

	bool OpenGLContext::init()
	{
		if (!glfwInit())
		{
			return false;
		}

		struct WindowInfo
		{
			char title[128];
			int width;
			int height;
			int majorVersion;
			int minorVersion;
			int samples;
		};

		WindowInfo windowInfo;
		memcpy(&windowInfo.title, "EGEFramework v0.1 OpenGL", sizeof("EGEFramework v0.1 OpenGL"));
		windowInfo.majorVersion = 4;
		windowInfo.minorVersion = 3;
		windowInfo.samples = 0;
		windowInfo.height = 720;
		windowInfo.width = 1280;

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, windowInfo.majorVersion);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, windowInfo.minorVersion);
	#ifdef _DEBUG
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
	#endif
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		glfwWindowHint(GLFW_SAMPLES, windowInfo.samples);
		glfwWindowHint(GLFW_STEREO, GL_FALSE);

		m_window = glfwCreateWindow(
			windowInfo.width,
			windowInfo.height,
			windowInfo.title,
			nullptr,
			nullptr);
		glfwMakeContextCurrent(m_window);

		gl3wInit();

	#ifdef _DEBUG
		fprintf(stderr, "VENDOR: %s\n", (char *)glGetString(GL_VENDOR));
		fprintf(stderr, "VERSION: %s\n", (char *)glGetString(GL_VERSION));
		fprintf(stderr, "RENDERER: %s\n", (char *)glGetString(GL_RENDERER));

		if (gl3wIsSupported(4, 3))
		{
			glDebugMessageCallback((GLDEBUGPROC)debugCallback, this);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		}
		else if (IsExtensionSupported("GL_ARB_debug_output"))
		{
			glDebugMessageCallbackARB((GLDEBUGPROC)debugCallback, this);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
		}
	#endif

		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		// Set input callbacks
		glfwSetKeyCallback(m_window, Input::glfw_onKey);
		glfwSetCursorPosCallback(m_window, Input::glfw_onMouseMove);

		// Init defaults
		GLuint shaders[2] = {
			ShaderUtil::loadShader("assets/shaders/passthrough_fragment.gl", GL_FRAGMENT_SHADER),
			ShaderUtil::loadShader("assets/shaders/passthrough_vertex.gl", GL_VERTEX_SHADER) };
		m_defaultShader = new RenderProgram();
		m_defaultShader->create(shaders, 2);
		glUseProgram(m_defaultShader->getProgram());

		GLuint vertexArrays[1] = { m_vertexArray };
		glGenVertexArrays(1, vertexArrays);
		glBindVertexArray(vertexArrays[0]);

		return true;
	}

	void OpenGLContext::destroy()
	{
		glfwDestroyWindow(m_window);
		glfwTerminate();
	}

	void OpenGLContext::debugCallback(GLenum src, GLenum type, GLuint id, GLenum priority, GLsizei length, const GLchar* msg, GLvoid* userParams)
	{
		// TODO: Handle debug callback
	#ifdef _WIN32
		OutputDebugStringA(msg);
		OutputDebugStringA("\n");
	#endif /* _WIN32 */
	}
	
	void OpenGLContext::swapBuffers()
	{
		glfwSwapBuffers(m_window);
	}

	void OpenGLContext::pollEvents()
	{
		glfwPollEvents();
	}

	double OpenGLContext::getTime()
	{
		return glfwGetTime();
	}
}