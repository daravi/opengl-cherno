#include "VertexArray.h"

#include <GL/glew.h>

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

VertexArray::VertexArray()
{
	glGenVertexArrays(1, &m_rendererID);
}

VertexArray::~VertexArray()
{
	unbind();
	glDeleteVertexArrays(1, &m_rendererID);
}

void VertexArray::addObject(std::unique_ptr<VertexBuffer> vb, const VertexBufferLayout& layout)
{
	m_vertexBuffer = std::move(vb);
	bind();
	m_vertexBuffer->bind();
	const auto& elements = layout.elements();
	unsigned int offset{ 0 };
	for (unsigned int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		glVertexAttribPointer(i, element.count, element.type, element.normalize, layout.stride(), (const void*)(size_t)offset);
		offset += element.size();
		glEnableVertexAttribArray(i);
	}
	
}

void VertexArray::bind() const
{
	glBindVertexArray(m_rendererID);
}

void VertexArray::unbind() const
{
	glBindVertexArray(0);
}