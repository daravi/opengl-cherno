#pragma once

#include <vector>

#include <GL/glew.h>

namespace
{
	unsigned int sizeOfType(unsigned int type)
	{
		switch (type)
		{
			case GL_FLOAT:         return 4;
			case GL_UNSIGNED_INT:  return 4;
			case GL_UNSIGNED_BYTE: return 1;
			default:
				throw std::runtime_error("Unsupported type");
		}
		return 0;
	}
}

struct VertexBufferLayoutElement
{
	unsigned int type;
	unsigned int count;
	unsigned char normalize;

	unsigned int size() const
	{
		return sizeOfType(type) * count;
	}
};


class VertexBufferLayout
{
private:
	std::vector<VertexBufferLayoutElement> m_elements;
	unsigned int m_stride{ 0 };

public:
	VertexBufferLayout() = default;
	~VertexBufferLayout() = default;

	template<typename T>
	void push(unsigned int count)
	{
		// TODO PUYA: Better way to do this?
		static_assert(!std::is_same_v<T,T>);
	}

	template<>
	void push<float>(unsigned int count)
	{
		m_elements.push_back({ GL_FLOAT, count, GL_FALSE });
		m_stride += count * sizeOfType(GL_FLOAT);
	}

	template<>
	void push<unsigned int>(unsigned int count)
	{
		m_elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		m_stride += count * sizeOfType(GL_UNSIGNED_INT);
	}

	template<>
	void push<unsigned char>(unsigned int count)
	{
		m_elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
		m_stride += count * sizeOfType(GL_UNSIGNED_BYTE);
	}

	const std::vector<VertexBufferLayoutElement>& elements() const { return m_elements; }
	unsigned int stride() const { return m_stride; }
};
