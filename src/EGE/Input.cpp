#include <EGE\Input.h>
#include <string>
namespace EGE
{
	Input* Input::m_instance = nullptr;

	Input::Input()
	{
		memset(m_key, 0, sizeof(m_key));
		memset(m_heldKey, 0, sizeof(m_heldKey));
	}

	Input* Input::getInstance()
	{
		if (!m_instance)
		{
			m_instance = new Input();
			return m_instance;
		}
		else
		{
			return m_instance;
		}
	}

	bool Input::getKey(int key)
	{
		return m_key[key];
	}

	bool Input::getKeyHeld(int key)
	{
		return m_heldKey[key];
	}

	void Input::glfw_onKey(GLFWwindow*, int key, int scancode, int action, int mods)
	{
		switch (action)
		{
		case GLFW_PRESS:
		{
			auto input = Input::getInstance();
			input->m_key[key] = true;
		}
		break;
		case GLFW_RELEASE:
		{
			auto input = Input::getInstance();
			input->m_key[key] = false;
			input->m_heldKey[key] = false;
		}
		break;
		case GLFW_REPEAT:
		{
			auto input = Input::getInstance();
			input->m_heldKey[key] = true;
		}
		break;
		default:
			break;
		}
	}

	void Input::glfw_onMouseMove(GLFWwindow*, double x, double y)
	{
		auto& input = *Input::getInstance();
		vmath::vec2 oldMousePos = input.m_mousePos;
		input.m_mousePos = vmath::vec2(static_cast<float>(x), static_cast<float>(y));
		input.m_mouseDelta = input.m_mousePos - oldMousePos;
	}
}

