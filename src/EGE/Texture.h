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

		int getWidth() { return width_; }
		int getHeight() { return height_; }

	private:
		GLuint texture_;
		int width_, height_;
	};
}