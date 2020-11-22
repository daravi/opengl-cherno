#include <array>

#include <GL/glew.h> // has to be included before GLFW. TODO PUYA: Abstract away both
#include <GLFW/glfw3.h>

#include <imgui.h>
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Renderer.h"
#include "tests/TestClearColor.h"
#include "tests/TestTexture2D.h"
#include "tests/TestMenu.h"

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
    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    test::TestMenu testMenu;
    testMenu.registerTest<test::TestClearColor>("Clear Color");
    testMenu.registerTest<test::TestTexture2D>("Texture 2D");
    test::TestBase* currentWindow{ &testMenu };

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        renderer.clear();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if ((currentWindow = testMenu.currentTest()))
        {
            currentWindow->onUpdate(0.0f);
            currentWindow->onRender();
            ImGui::Begin(testMenu.currentTestName().c_str());
            if (ImGui::Button("<-"))
            {
                testMenu.clearTest();
                currentWindow = &testMenu;
            }
            currentWindow->onImGuiRender();
            ImGui::End();
        }
        else
        {
            currentWindow = &testMenu;
            ImGui::Begin("Test Selection");
            currentWindow->onImGuiRender();
            ImGui::End();
        }
        

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}