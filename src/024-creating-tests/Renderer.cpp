#include "Renderer.h"

#include <iostream>
#include <stdexcept>

#include <fmt/format.h>
#include <GL/glew.h>

#include "IndexBuffer.h"
#include "Shader.h"
#include "VertexArray.h"

namespace
{
    void _glEnableDebug()
    {
        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback([](GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam)
        {
            switch (severity)
            {
                case GL_DEBUG_SEVERITY_LOW:
                    std::cout << "[OpenGL::INFO] " << message << std::endl;
                    break;
                case GL_DEBUG_SEVERITY_MEDIUM:
                    std::cout << "[OpenGL::WARNING] " << message << std::endl;
                    break;
                case GL_DEBUG_SEVERITY_HIGH:
                    std::cerr << "[OpenGL::ERROR] " << message << std::endl;
                    break;
            }
        }, nullptr);
    }
}

Renderer::Renderer()
{
	auto err = glewInit();
	if (err != GLEW_OK)
	{
		throw std::runtime_error(fmt::format("Error initializing GLEW: {}", glewGetErrorString(err)));
	}

	// gl* functions below are loaded(i.e. resolved) by GLEW
	#ifdef NDEBUG
		// nondebug code
	#else
		// debug code
		_glEnableDebug();
	#endif

	std::cout << glGetString(GL_VERSION) << std::endl;

    // Blending determines how we combine our output color (in fragment shader, aka source) with what is already in our target buffer (what has already been rendered, aka destination)
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // source and destination are multiplication facotrs when combining RGBA of source and destination
    glBlendEquation(GL_FUNC_ADD);                      // by default blend equation mode is GL_FUNC_ADD: result = source_rgba * alpha_src + destination_rgba * (1 - alpha_src)
    glEnable(GL_BLEND);
}

void Renderer::clear() const
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
	shader.bind();
	va.bind();
	ib.bind();
	glDrawElements(GL_TRIANGLES, ib.count(), GL_UNSIGNED_INT, nullptr);
}