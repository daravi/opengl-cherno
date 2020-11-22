#include <array>

#include <GL/glew.h> // has to be included before GLFW. TODO PUYA: Abstract away both
#include <GLFW/glfw3.h>

#include "IndexBuffer.h"
#include "Renderer.h"
#include "Shader.h"
#include "Texture.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

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

    Renderer renderer; // Does the GLEW initialization

    // Adding texture coordinates for each vertex (texture coordinates are normalized between 0,0 and 1,1)
    constexpr std::array positions{
        -0.5f, -0.5f, 0.0f, 0.0f, // 0
         0.5f, -0.5f, 1.0f, 0.0f, // 1
         0.5f,  0.5f, 1.0f, 1.0f, // 2
        -0.5f,  0.5f, 0.0f, 1.0f  // 3
    };
    VertexArray va;
    VertexBuffer vb(positions.data(), positions.size() * sizeof(float));
    VertexBufferLayout layout;
    layout.push<float>(2); // attribute index/location 0
    layout.push<float>(2); // attribute index/location 1
    va.addObject(vb, std::move(layout));

    constexpr std::array indices{
        0u, 1u, 2u,
        2u, 3u, 0u
    };
    IndexBuffer ib(indices.data(), indices.size());

    // You need to run the binary from lesson directory '016-textures' for these relative paths to work
    Shader shader("res/shaders/vertex.glsl", "res/shaders/fragment.glsl");
    renderer.draw(va, ib, shader);

    Texture texture("../../res/images/cherno.png");
    texture.bind();
    shader.setUniform1i("u_texture", 0); // should match the bound texture slot

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        renderer.clear();
        renderer.draw(va, ib, shader);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}