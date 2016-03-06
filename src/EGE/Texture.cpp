#include <EGE\Texture.h>

#include <assert.h>

namespace EGE
{
	Texture::Texture()
	{

	}

	Texture::~Texture()
	{
		glDeleteTextures(1, &m_texture);
	}

	void Texture::create(const char* file)
	{
		int components;
		auto data = stbi_load(file, &m_width, &m_height, &components, 4);
		glGenTextures(1, &m_texture);
		glBindTexture(GL_TEXTURE_2D, m_texture);
		glTextureParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		stbi_image_free(data);
	}

	void Texture::bind(unsigned slot)
	{
		assert(slot < GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS && "slot < GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS");
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, m_texture);
	}
}