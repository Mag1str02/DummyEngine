// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <imgui.h>

#include <DummyEngine/Utils/Debug/Assert.h>
#include <DummyConcurrency/DummyConcurrency.hpp>

using namespace DummyEngine;
using namespace NDummyConcurrency;
using namespace std::chrono_literals;

static void ErrorCallback(int, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
    fflush(stderr);
}

class GLFWThread {
public:
    GLFWThread() : thread_pool_(1) {}
    ~GLFWThread() {
        if (!stop_flag_) {
            Stop();
        }
    }
    void Start() {
        Submit(thread_pool_, [this]() { InitGlfw(); });

        thread_pool_.Start();
    }
    void StartPollingEvents() {
        Submit(thread_pool_, [this]() { ProcessEvents(); });
    }
    void Stop() {
        stop_flag_.store(true);
        stopped_event_processing_.Wait();

        Submit(thread_pool_, []() { glfwTerminate(); });

        thread_pool_.Stop();
    }

    Future<GLFWwindow*> CreateWindow() {
        return NFuture::Submit(thread_pool_, []() {
            auto* window = glfwCreateWindow(1280, 720, "Window", nullptr, nullptr);
            DE_ASSERT(window, "Failed to craete window");
            return window;
        });
    }
    void DestroyWindow(GLFWwindow* window) {
        Submit(thread_pool_, [window]() { glfwDestroyWindow(window); });
    }

private:
    void InitGlfw() {
        if (glfwInit() == GLFW_FALSE) {
            DE_ASSERT(false, "Failed to initialize GLFW");
        }

        glfwSetErrorCallback(ErrorCallback);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    }
    void ProcessEvents() {
        if (!stop_flag_) {
            glfwPollEvents();  // TODO: wait
            Submit(thread_pool_, [this]() { ProcessEvents(); });
        } else {
            stopped_event_processing_.Fire();
        }
    }

private:
    ThreadPool thread_pool_;

    std::atomic<bool> stop_flag_ = false;
    TEvent            stopped_event_processing_;
};

int main() {
    GLFWThread glfw;

    glfw.Start();
    auto* window = NFuture::Get(glfw.CreateWindow());
    glfwMakeContextCurrent(window);
    DE_ASSERT(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress), "Failed to load glad");

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;      // Enable Docking
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 410");

    glfw.StartPollingEvents();
    while (glfwWindowShouldClose(window) != GLFW_TRUE) {
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

    glfw.DestroyWindow(window);
    glfw.Stop();
}
