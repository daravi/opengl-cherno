#include <array>

#include <GL/glew.h> // has to be included before GLFW. TODO PUYA: Abstract away both
#include <GLFW/glfw3.h>

// A projection matrix tells our Window how to map the vertices, i.e. how to project 3D geometry onto the 2D display
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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
    window = glfwCreateWindow(960, 540, "Hello World", NULL, NULL);
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
    // constexpr std::array positions{
    //     -0.5f, -0.5f, 0.0f, 0.0f, // 0
    //      0.5f, -0.5f, 1.0f, 0.0f, // 1
    //      0.5f,  0.5f, 1.0f, 1.0f, // 2
    //     -0.5f,  0.5f, 0.0f, 1.0f  // 3
    // };
    constexpr std::array positions{
        230.0f, 50.0f,  0.0f, 0.0f, // 0
        730.0f, 50.0f,  1.0f, 0.0f, // 1
        730.0f, 490.0f, 1.0f, 1.0f, // 2
        230.0f, 490.0f, 0.0f, 1.0f  // 3
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

    // orthographic (2D) vs perspective (3D, i.e. size depends on z) projection
    glm::mat4 projection = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);

    // You need to run the binary from lesson directory '018-projection' for these relative paths to work
    Shader shader("res/shaders/vertex.glsl", "res/shaders/fragment.glsl");
    renderer.draw(va, ib, shader);

    Texture texture("../../res/images/cherno.png");
    texture.bind();
    shader.setUniform1i("u_texture", 0); // should match the bound texture slot
    shader.setUniformMat4f("u_projection", projection);

    float r{ 0.0f }; // red channel
    float increment{ 0.05f };

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        renderer.clear();
        renderer.draw(va, ib, shader);

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