#pragma once

class VertexBuffer
{
public:
	VertexBuffer(const void* data, unsigned int size);
	VertexBuffer(const VertexBuffer& other) = delete;
	VertexBuffer& operator=(const VertexBuffer& other) = delete;
	~VertexBuffer();

	void bind() const;
	void unbind() const;

private:
	unsigned int m_rendererID;
};