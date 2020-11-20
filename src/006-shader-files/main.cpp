#include <array>
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

    auto err = glewInit();
    if (err != GLEW_OK)
    {
        std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
        return -1;
    }

    // gl* functions below are loaded(i.e. resolved) by GLEW

    std::cout << glGetString(GL_VERSION) << std::endl;

    constexpr std::array positions{
        -0.5f, -0.5f,
         0.0f,  0.5f,
         0.5f, -0.5f
    };

    // Creat a vertex buffer
    unsigned int buffer_id;
    glGenBuffers(1, &buffer_id);
    glBindBuffer(GL_ARRAY_BUFFER, buffer_id); // Select the buffer for the OpenGL global state-machine
    glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(float), positions.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

    // You need to run the binary from lesson directory '006-shaders-in-opengl' for these relative paths to work
    std::string vertexShader{ readFile("res/shaders/vertex.glsl") };
    std::string fragmentShader{ readFile("res/shaders/fragment.glsl") };
    auto program_id = createProgram(vertexShader, fragmentShader);
    glUseProgram(program_id); // Bind our shader program in the global OpenGL state-machine

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteProgram(program_id);

    glfwTerminate();
    return 0;
}