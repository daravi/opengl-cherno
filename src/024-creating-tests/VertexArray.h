#pragma once

#include <memory>

class VertexBuffer;
class VertexBufferLayout;

class VertexArray
{
private:
	unsigned int m_rendererID;
	std::unique_ptr<VertexBuffer> m_vertexBuffer{ nullptr };

public:
	VertexArray();
	~VertexArray();

	// TODO PUYA: Should be able to take multiple layouts for a single buffer
	void addObject(std::unique_ptr<VertexBuffer> vb, const VertexBufferLayout& layout);

	void bind() const;
	void unbind() const;
};
