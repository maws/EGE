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

	private:
		GLuint texture_;
	};
}