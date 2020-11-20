#include "Texture.h"

#include <GL/glew.h>

#define STB_IMAGE_IMPLEMENTATION // This should pre-include definition should only appear in *one* translation unit
#include <stb_image.h>

Texture::Texture(std::string_view filepath)
{
	stbi_set_flip_vertically_on_load(true);
	auto localBuffer = stbi_load(filepath.data(), &m_width, &m_height, &m_bpp, 4);
	
	glGenTextures(1, &m_renderedID);
	glBindTexture(GL_TEXTURE_2D, m_renderedID);

	// These four parameters are generally required for any texture rendering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // target, param, value
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, localBuffer);
	glBindTexture(GL_TEXTURE_2D, 0);

	if (localBuffer)
	{
		stbi_image_free(localBuffer);
	}
}

Texture::~Texture()
{
	unbind();
	glDeleteTextures(1, &m_renderedID);
}

void Texture::bind(unsigned int slot) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, m_renderedID);
}

void Texture::unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

int Texture::width() const
{
	return m_width;
}

int Texture::height() const
{
	return m_height;
}