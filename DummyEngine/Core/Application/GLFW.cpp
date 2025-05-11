#include "GLFW.h"

#include <GLFW/glfw3.h>

namespace DummyEngine {

    namespace {
        GLFWmonitor* GetMonitor(U32 id) {
            int           monitors_amount = 0;
            GLFWmonitor** monitors        = glfwGetMonitors(&monitors_amount);
            DE_ASSERT(id < U32(monitors_amount), "Wrong monitor id {} should be between [0, {})", id, monitors_amount);
            return monitors[id];
        }
        void ErrorCallback(int, const char* description) {
            fprintf(stderr, "GLFW Error: %s\n", description);
            fflush(stderr);
        }

    }  // namespace

    SINGLETON_BASE(GLFW);
    S_INITIALIZE() {
        Submit(thread_pool_, [this]() { InitGLFW(); });

        thread_pool_.Start();
        return Unit();
    }
    S_TERMINATE() {
        stop_flag_.store(true);
        stopped_event_processing_.Wait();

        Submit(thread_pool_, []() { glfwTerminate(); });

        thread_pool_.Stop();
        return Unit();
    }

    S_METHOD_IMPL(Unit, StartEventProcessing, (), ()) {
        Submit(thread_pool_, [this]() { ProcessEvents(); });
        return Unit();
    }
    S_METHOD_IMPL(Unit, StopEventProcessing, (), ()) {
        stop_flag_.store(true);
        stopped_event_processing_.Wait();
        return Unit();
    }

    S_METHOD_IMPL(Future<GLFWwindow*>, CreateWindow, (), ()) {
        return NFuture::Submit(thread_pool_, []() -> GLFWwindow* {
            auto* window = glfwCreateWindow(1280, 720, "Window", nullptr, nullptr);
            DE_ASSERT(window, "Failed to craete window");
            return window;
        });
    }
    S_METHOD_IMPL(Future<Unit>, DestroyWindow, (GLFWwindow * window), (window)) {
        return NFuture::Submit(thread_pool_, [window]() {
            glfwDestroyWindow(window);
            return Unit();
        });
    }
    S_METHOD_IMPL(Future<Unit>, EnableFullScreen, (GLFWwindow * window, U32 monitor_id), (window, monitor_id)) {
        return NFuture::Submit(thread_pool_, [window, monitor_id]() {
            GLFWmonitor*       monitor = GetMonitor(monitor_id);
            const GLFWvidmode* mode    = glfwGetVideoMode(monitor);
            glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
            return Unit();
        });
    }
    S_METHOD_IMPL(Future<Unit>, DisableFullScreen, (GLFWwindow * window), (window)) {
        return NFuture::Submit(thread_pool_, [window]() {
            glfwSetWindowMonitor(window, nullptr, 100, 100, 1280, 720, 1000);
            return Unit();
        });
    }
    S_METHOD_IMPL(Future<Unit>, SetCursorMode, (GLFWwindow * window, U32 mode), (window, mode)) {
        return NFuture::Submit(thread_pool_, [window, mode]() {
            glfwSetInputMode(window, GLFW_CURSOR, mode);
            return Unit();
        });
    }

    void GLFW::InitGLFW() {
        if (glfwInit() == GLFW_FALSE) {
            DE_ASSERT(false, "Failed to initialize GLFW");
        }
        glfwSetErrorCallback(ErrorCallback);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    }

    void GLFW::ProcessEvents() {
        if (!stop_flag_) {
            glfwPollEvents();  // TODO: some wait required
            Submit(thread_pool_, [this]() { ProcessEvents(); });
        } else {
            stopped_event_processing_.Fire();
        }
    }
}  // namespace DummyEngine
