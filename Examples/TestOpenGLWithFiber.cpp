// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <imgui.h>

#include <DummyEngine/Utils/Debug/Assert.h>
#include <DummyEngine/Utils/PCH/DC.h>
#include <DummyConcurrency/DummyConcurrency.hpp>

using namespace DummyEngine;
using namespace std::chrono_literals;

static void ErrorCallback(int, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
    fflush(stderr);
}

int main() {
    if (glfwInit() == GLFW_FALSE) {
        DE_ASSERT(false, "Failed to initialize GLFW");
    }

    glfwSetErrorCallback(ErrorCallback);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    RunLoop loop;
    Futures::Go(loop, [&]() {
        auto* window = glfwCreateWindow(1280, 720, "Window", nullptr, nullptr);
        DE_ASSERT(window, "Failed to craete window");

        glfwMakeContextCurrent(window);
        DE_ASSERT(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress), "Failed to load glad");

        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;      // Enable Docking
        ImGui::StyleColorsDark();

        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 410");

        while (glfwWindowShouldClose(window) != GLFW_TRUE) {
            glfwPollEvents();
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            ImGui::ShowDemoWindow();

            int display_w, display_h;
            glfwGetFramebufferSize(window, &display_w, &display_h);
            glViewport(0, 0, display_w, display_h);
            glClearColor(0, 0, 0, 1);
            glClear(GL_COLOR_BUFFER_BIT);

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            glfwSwapBuffers(window);
        }

        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        glfwMakeContextCurrent(nullptr);
        glfwDestroyWindow(window);
        return Unit();
    }) | Futures::Detach();

    loop.Run();
}
