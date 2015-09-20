#include <EGE\Texture.h>

namespace EGE
{
	Texture::Texture()
	{

	}

	Texture::~Texture()
	{
		glDeleteTextures(1, &texture_);
	}

	void Texture::create(const char* file)
	{
		int width;
		int height;
		int components;

		auto data = stbi_load(file, &width, &height, &components, 4);
		glGenTextures(1, &texture_);
		glBindTexture(GL_TEXTURE_2D, texture_);
		glTextureParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		stbi_image_free(data);
	}

	void Texture::bind(unsigned slot)
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, texture_);
	}
}