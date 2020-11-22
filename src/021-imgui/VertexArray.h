#pragma once

class VertexBuffer;
class VertexBufferLayout;

class VertexArray
{
private:
	unsigned int m_rendererID;

public:
	VertexArray();
	~VertexArray();

	// TODO PUYA: Should be able to take multiple layouts for a single buffer
	void addObject(const VertexBuffer& vb, const VertexBufferLayout& layout);

	void bind() const;
	void unbind() const;
};
