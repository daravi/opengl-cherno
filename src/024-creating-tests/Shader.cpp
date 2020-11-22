#include "Shader.h"

#include <climits>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string_view>

#include <GL/glew.h>
#include <glm/glm.hpp>

namespace
{
	std::string readFile(std::string_view filepath)
    {
        std::ifstream fs(filepath);

        std::stringstream buffer;
        buffer << fs.rdbuf();

        return buffer.str();
    }

	unsigned int compileShader(unsigned int type, std::string_view source)
    {
        auto id = glCreateShader(type);
        auto source_ptr = source.data();
        glShaderSource(id, 1, &source_ptr, nullptr);
        glCompileShader(id);

        int result;
        glGetShaderiv(id, GL_COMPILE_STATUS, &result);
        if (result == GL_FALSE)
        {
            int length;
            glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
            char message[length];
            // message.reserve(length);
            glGetShaderInfoLog(id, INT_MAX, &length, &message[0]);

            std::cerr << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader:\n" << message << std::endl;

            glDeleteShader(id);
            return 0;
        }

        return id;
    }

	unsigned int createProgram(std::string_view vertexShader, std::string_view fragmentShader)
    {
        auto program_id = glCreateProgram();
        auto vertex_shader_id = compileShader(GL_VERTEX_SHADER, vertexShader);
        auto fragment_shader_id = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

        glAttachShader(program_id, vertex_shader_id);
        glAttachShader(program_id, fragment_shader_id);
        glLinkProgram(program_id);
        glValidateProgram(program_id); // Checks to see whether the executables contained in program can execute given the current OpenGL state (useful during development, not production code)

        glDeleteShader(vertex_shader_id);
        glDeleteShader(fragment_shader_id);

        return program_id;
    }
}

Shader::Shader(std::string_view vertex_shader_filepath, std::string_view fragment_shader_filepath)
{
	// Should I keep a copy?
	std::string vertexShader{ readFile("res/shaders/vertex.glsl") };
    std::string fragmentShader{ readFile("res/shaders/fragment.glsl") };
	m_rendererID = createProgram(vertexShader, fragmentShader);
}

Shader::~Shader()
{
	unbind();
	glDeleteProgram(m_rendererID);
}


void Shader::bind() const
{
	glUseProgram(m_rendererID);
}

void Shader::unbind() const
{
	glUseProgram(0);
}

void Shader::setUniform1i(const std::string& name, int v)
{
    int location = uniformLocation(name);
	if (location == -1)
	{
		std::cerr << "Shader::setUniform1i -- Warning: Cannot set uniform " << name << " since it is not compiled into shader." << std::endl;
	}
    else
    {
        glUniform1i(uniformLocation(name), v);
    }
}

void Shader::setUniform4f(const std::string& name, const std::array<float, 4>& v)
{
	int location = uniformLocation(name);
	if (location == -1)
	{
		std::cerr << "Shader::setUniform4f -- Warning: Cannot set uniform " << name << " since it is not compiled into shader." << std::endl;
	}
    else
    {
        glUniform4f(location, v[0], v[1], v[2], v[3]);
    }
}

void Shader::setUniformMat4f(const std::string& name, const glm::mat4& v)
{
    int location = uniformLocation(name);
	if (location == -1)
	{
		std::cerr << "Shader::setUniformMat4f -- Warning: Cannot set uniform " << name << " since it is not compiled into shader." << std::endl;
	}
    else
    {
        glUniformMatrix4fv(location, 1, GL_FALSE, &v[0][0]); // we can pass pointer to first element since glm::mat4 is column major as openGL expects
    }
}

int Shader::uniformLocation(const std::string& name) const
{
	if (!m_uniformLocationCache.contains(name))
	{
		m_uniformLocationCache[name] = glGetUniformLocation(m_rendererID, name.data());
	}
	return m_uniformLocationCache[name];
}
