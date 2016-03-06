#pragma once

#define STD_IMAGE_IMPLEMENTATION
#include <stb\stb_image.h>
#include <EGE\Application.hpp>

namespace EGE
{
	class Texture
	{
	public:
		Texture();
		~Texture();

		void create(const char* file);
		void bind(unsigned slot);
		int getWidth() { return m_width; }
		int getHeight() { return m_height; }

	private:
		GLuint m_texture;
		int m_width;
		int m_height;
	};
}