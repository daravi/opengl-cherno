#pragma once

class IndexBuffer;
class Shader;
class VertexArray;

class Renderer
{
public:
	Renderer();
	~Renderer() = default;
	void clear() const;
	void draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
};