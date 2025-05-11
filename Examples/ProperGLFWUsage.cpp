// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

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
        Submit(thread_pool_, [this]() {
            InitGlfw();
            ProcessEvents();
        });

        thread_pool_.Start();
    }
    void Stop() {
        stop_flag_.store(true);

        Submit(thread_pool_, []() { glfwTerminate(); });

        thread_pool_.Stop();
    }

    Future<GLFWwindow*> CreateWindow() {
        return NFuture::Submit(thread_pool_, []() {
            auto* window = glfwCreateWindow(1280, 720, "Window", nullptr, nullptr);

            glfwMakeContextCurrent(window);
            gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
            glfwMakeContextCurrent(nullptr);
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
        }
    }

private:
    ThreadPool thread_pool_;

    ManualLifetime<std::thread> thread_;
    std::atomic<bool>           stop_flag_ = false;
};

int main() {
    GLFWThread glfw;

    glfw.Start();
    auto* window = NFuture::Get(glfw.CreateWindow());
    glfwMakeContextCurrent(window);
    while (glfwWindowShouldClose(window) != GLFW_TRUE) {
        const float v = (float)fabs(sin(glfwGetTime() * 2.f));
        glClearColor(v, 1.0f, 0.0f, 0.f);

        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);
    }
    glfwMakeContextCurrent(nullptr);

    glfw.DestroyWindow(window);
    glfw.Stop();
}
