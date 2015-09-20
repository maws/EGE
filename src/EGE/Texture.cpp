#include <EGE\Texture.h>

#include <assert.h>

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
		int components;
		auto data = stbi_load(file, &width_, &height_, &components, 4);
		glGenTextures(1, &texture_);
		glBindTexture(GL_TEXTURE_2D, texture_);
		glTextureParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width_, height_, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		stbi_image_free(data);
	}

	void Texture::bind(unsigned slot)
	{
		assert(slot < GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS && "slot < GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS");
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, texture_);
	}
}