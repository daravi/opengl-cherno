#include <array>
#include <cassert>
#include <climits>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

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
            std::string message;
            message.reserve(length);
            glGetShaderInfoLog(id, INT_MAX, &length, message.data());

            std::cerr << "Failed to compile " << (type == GL_VERTEX_SHADER ? "Vertex" : "Fragment") << ":\n" << message << std::endl;

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

int main(int argc, char const *argv[])
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

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

    // Creat a vertex buffer
    unsigned int vertex_buffer_id;
    glGenBuffers(1, &vertex_buffer_id);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);
    glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(float), positions.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

    // Creat an index buffer
    unsigned int index_buffer_id;
    glGenBuffers(1, &index_buffer_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // You need to run the binary from lesson directory '010-uniforms' for these relative paths to work
    std::string vertexShader{ readFile("res/shaders/vertex.glsl") };
    std::string fragmentShader{ readFile("res/shaders/fragment.glsl") };
    auto program_id = createProgram(vertexShader, fragmentShader);
    glUseProgram(program_id); // Bind our shader program in the global OpenGL state-machine

    int location = glGetUniformLocation(program_id, "u_color"); // returns -1 if u_color uniform is not compiled into the shader program
    assert(location != -1);

    float r{ 0.0f }; // red channel
    float increment{ 0.05f };

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        // Uniforms are passed per draw (as opposed to attributes which are per vertex)
        glUniform4f(location, r, 0.3f, 0.8f, 1.0f);
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

    glDeleteProgram(program_id);

    glfwTerminate();
    return 0;
}