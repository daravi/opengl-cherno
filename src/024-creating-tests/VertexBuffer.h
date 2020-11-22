#pragma once

class VertexBuffer
{
public:
	VertexBuffer(const void* data, unsigned int size);
	VertexBuffer(const VertexBuffer& other) = delete;
	VertexBuffer& operator=(const VertexBuffer& other) = delete;
	VertexBuffer(VertexBuffer&& other) = default;
	VertexBuffer& operator=(VertexBuffer&& other) = default;
	~VertexBuffer();

	void bind() const;
	void unbind() const;

private:
	unsigned int m_rendererID;
};