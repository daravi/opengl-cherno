#include <array>

#include <GL/glew.h> // has to be included before GLFW. TODO PUYA: Abstract away both
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

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

    // Dear ImGui setup
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    // ImGuiIO &io = ImGui::GetIO();
    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Adding texture coordinates for each vertex (texture coordinates are normalized between 0,0 and 1,1)
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

    glm::vec3 translation(0.0f, 0.0f, 0.0f);

    glm::mat4 projection = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-100, 0, 0)); // Same as moving the camera to right by +100
    glm::mat4 model = glm::translate(glm::mat4(1.0f), translation);
    
    glm::mat4 vp = projection * view; // Ok to do on CPU since for now camera is not moving (i.e. multiplication is done only once)

    // You need to run the binary from lesson directory '021-imgui' for these relative paths to work
    Shader shader("res/shaders/vertex.glsl", "res/shaders/fragment.glsl");
    renderer.draw(va, ib, shader);

    Texture texture("../../res/images/cherno.png");
    texture.bind();
    shader.setUniform1i("u_texture", 0); // should match the bound texture slot
    shader.setUniformMat4f("u_mvp", vp * model);



    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        renderer.clear();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        renderer.draw(va, ib, shader);

        {
            // This works since x,y,z are laid contigiously in memory. TOOD PUYA: Is this garaungeed?
            ImGui::SliderFloat2("position", &translation.x, -0.500f, 500.0f); // Edit 2 floats using a slider from 0.0f to 1000.0f (x and y, we don't care about z at the moment)
        }

        model = glm::translate(glm::mat4(1.0f), translation);
        shader.setUniformMat4f("u_mvp", vp * model);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}