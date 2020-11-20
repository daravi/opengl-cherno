#pragma once

#include <string_view>

class Texture
{
private:
	unsigned int m_renderedID{ 0 };
	int m_width{ 0 }, m_height{ 0 }, m_bpp{ 0 }; // bpp is bits per pixel

public:
	Texture(std::string_view filepath);
	~Texture();

	void bind(unsigned int slot = 0) const;
	void unbind() const;

	int width() const;
	int height() const;
};
