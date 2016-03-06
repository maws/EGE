#pragma once

#include <GLFW\glfw3.h>
#include <vmath.h>

namespace EGE
{
	class Input
	{
	public:
		friend class OpenGLContext;

		static Input* getInstance();

		bool getKey(int key);
		bool getKeyHeld(int key);
		const vmath::vec2& getMousePos() { return m_mousePos; }
		const vmath::vec2& getMouseDelta() { return m_mouseDelta; }

	protected:
		Input();

	private:
		static Input* m_instance;
		bool m_key[GLFW_KEY_LAST];
		bool m_heldKey[GLFW_KEY_LAST];
		vmath::vec2 m_mousePos;
		vmath::vec2 m_mouseDelta;

		// GLFW callbacks
		static void glfw_onKey(GLFWwindow*, int key, int scancode, int action, int mods);
		static void glfw_onMouseMove(GLFWwindow*, double x, double y);
	};
}