#pragma once

#include <array>
#include <string>
#include <unordered_map>

#include <glm/fwd.hpp>

class Shader
{
private:
	unsigned int m_rendererID{ 0 };
	mutable std::unordered_map<std::string, int> m_uniformLocationCache;

public:
	Shader(std::string_view vertex_shader_filepath, std::string_view fragment_shader_filepath);
	~Shader();

	void bind() const;
	void unbind() const;

	void setUniform1i(const std::string& name, int v);
	void setUniform4f(const std::string& name, const std::array<float, 4>& v);
	void setUniformMat4f(const std::string& name, const glm::mat4& v);

private:
	int uniformLocation(const std::string& name) const;
};
