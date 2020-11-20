#include <array>
#include <cassert>
#include <iostream>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "IndexBuffer.h"
#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

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

int main(int argc, char const *argv[])
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    auto err = glewInit();
    if (err != GLEW_OK)
    {
        std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
        return -1;
    }

    // gl* functions below are loaded(i.e. resolved) by GLEW
    #ifdef NDEBUG
        // nondebug code
    #else
        // debug code
        _glEnableDebug();
    #endif

    std::cout << glGetString(GL_VERSION) << std::endl;

    constexpr std::array positions{
        -0.5f, -0.5f, // 0
         0.5f, -0.5f, // 1
         0.5f,  0.5f, // 2
        -0.5f,  0.5f  // 3
    };
    constexpr std::array indices{
        0u, 1u, 2u,
        2u, 3u, 0u
    };

    VertexArray va;
    VertexBuffer vb(positions.data(), positions.size() * sizeof(float));
    VertexBufferLayout layout;
    layout.push<float>(2);
    va.addObject(vb, std::move(layout));

    // Creat an index buffer
    IndexBuffer ib(indices.data(), indices.size());

    // You need to run the binary from lesson directory '014-abstracting-shader' for these relative paths to work
    Shader shader("res/shaders/vertex.glsl", "res/shaders/fragment.glsl");

    float r{ 0.0f }; // red channel
    float increment{ 0.05f };

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        // Final flow using vertex arrays

        // 1 Bind shader
        shader.bind();

        // 2 Setup uniforms
        shader.setUniform4f("u_color", {r, 0.3f, 0.8f, 1.0f});
        // glUniform4f(location, r, 0.3f, 0.8f, 1.0f);

        // 3 Bind vertex array
        va.bind();

        // 4 Bind index buffer
        ib.bind();

        // 5 Draw
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        if (r >= 1.0f)
            increment = -0.05f;
        else if (r <= 0.0f)
            increment = 0.05f;
        
        r += increment;

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}