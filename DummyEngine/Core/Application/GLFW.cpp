#include "GLFW.h"

#include "Concurrency.h"

#include "DummyEngine/Utils/Debug/Profiler.h"

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
        NFuture::Submit(Concurrency::GetMainThreadScheduler(), [this]() {
            InitGLFW();
            return Unit();
        }) | NFuture::Get();

        return Unit();
    }
    S_TERMINATE() {
        stop_flag_.store(true);
        stopped_event_processing_.Wait();

        Submit(Concurrency::GetMainThreadScheduler(), []() { glfwTerminate(); });

        return Unit();
    }

    S_METHOD_IMPL(Unit, StartEventProcessing, (), ()) {
        Go(Concurrency::GetMainThreadScheduler(), [this]() {
            DE_PROFILE_SCOPE("GLFW::EventProcessing");
            while (!stop_flag_) {
                DE_PROFILE_SCOPE("GLFW::EventProcessing::Step");
                glfwPollEvents();
                NFiber::Yield();
            }
            stopped_event_processing_.Fire();
        });
        return Unit();
    }
    S_METHOD_IMPL(Unit, StopEventProcessing, (), ()) {
        stop_flag_.store(true);
        stopped_event_processing_.Wait();
        return Unit();
    }

    S_METHOD_IMPL(Future<GLFWwindow*>, CreateWindow, (), ()) {
        return NFuture::Submit(Concurrency::GetMainThreadScheduler(), [this]() -> GLFWwindow* {
            DE_PROFILE_SCOPE("GLFW::CreateWindow");
            auto* window = glfwCreateWindow(1280, 720, "Window", nullptr, nullptr);
            DE_ASSERT(window, "Failed to craete window");
            InstallCallbacks(window);

            std::lock_guard guard(events_mutex_);
            window_events_[window] = {};
            return window;
        });
    }
    S_METHOD_IMPL(Future<Unit>, DestroyWindow, (GLFWwindow * window), (window)) {
        return NFuture::Submit(Concurrency::GetMainThreadScheduler(), [this, window]() {
            DE_PROFILE_SCOPE("GLFW::DestroyWindow");
            glfwDestroyWindow(window);

            std::lock_guard guard(events_mutex_);
            window_events_.erase(window);
            return Unit();
        });
    }
    S_METHOD_IMPL(Future<Unit>, EnableFullScreen, (GLFWwindow * window, U32 monitor_id), (window, monitor_id)) {
        return NFuture::Submit(Concurrency::GetMainThreadScheduler(), [window, monitor_id]() {
            DE_PROFILE_SCOPE("GLFW::EnableFullScreen");
            GLFWmonitor*       monitor = GetMonitor(monitor_id);
            const GLFWvidmode* mode    = glfwGetVideoMode(monitor);
            glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
            return Unit();
        });
    }
    S_METHOD_IMPL(Future<Unit>, DisableFullScreen, (GLFWwindow * window), (window)) {
        return NFuture::Submit(Concurrency::GetMainThreadScheduler(), [window]() {
            DE_PROFILE_SCOPE("GLFW::DisableFullScreen");
            glfwSetWindowMonitor(window, nullptr, 100, 100, 1280, 720, 1000);
            return Unit();
        });
    }
    S_METHOD_IMPL(Future<Unit>, SetCursorMode, (GLFWwindow * window, U32 mode), (window, mode)) {
        return NFuture::Submit(Concurrency::GetMainThreadScheduler(), [window, mode]() {
            DE_PROFILE_SCOPE("GLFW::SetCursorMode");
            glfwSetInputMode(window, GLFW_CURSOR, mode);
            return Unit();
        });
    }
    S_METHOD_IMPL(std::vector<Event>, PullWindowEvents, (GLFWwindow * window), (window)) {
        DE_PROFILE_SCOPE("GLFW::PullEvents");

        std::lock_guard guard(events_mutex_);
        auto            it = window_events_.find(window);
        if (it == window_events_.end()) {
            return {};
        }

        return std::move(it->second);
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

        glfwSetMonitorCallback([](GLFWmonitor* monitor, int event) {
            GLFW::GetInstance().OnMonitorEvent(monitor, event);  //
        });
    }

    template <typename T>
    void GLFW::AddWindowEvent(GLFWwindow* window, T event) {
        event.Window = window;
        std::lock_guard guard(GetInstance().events_mutex_);
        auto            it = GetInstance().window_events_.find(window);
        if (it != GetInstance().window_events_.end()) {
            it->second.emplace_back(event);
        }
    }

    void GLFW::InstallCallbacks(GLFWwindow* window) {
        glfwSetCursorEnterCallback(window, [](GLFWwindow* window, int entered) {
            AddWindowEvent(window, WindowCursorEnteredEvent{.Entered = entered});  //
        });
        glfwSetWindowFocusCallback(window, [](GLFWwindow* window, int focused) {
            AddWindowEvent(window, WindowFocusEvent{.Focused = focused});  //
        });
        glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height) {
            AddWindowEvent(window, WindowResizeEvent{.Width = width, .Height = height});  //
        });
        glfwSetWindowCloseCallback(window, [](GLFWwindow* window) {
            AddWindowEvent(window, WindowCloseEvent{});  //
        });

        glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
            AddWindowEvent(window, KeyEvent{.Key = key, .Scancode = scancode, .Action = action, .Mods = mods});  //
        });
        glfwSetCharCallback(window, [](GLFWwindow* window, unsigned int c) {
            AddWindowEvent(window, CharEvent{.C = c});  //
        });

        glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods) {
            AddWindowEvent(window, MouseButtonEvent{.Button = button, .Action = action, .Mods = mods});  //
        });
        glfwSetScrollCallback(window, [](GLFWwindow* window, double xoffset, double yoffset) {
            AddWindowEvent(window, MouseScrolledEvent{.XOffset = xoffset, .YOffset = yoffset});  //
        });
        glfwSetCursorPosCallback(window, [](GLFWwindow* window, double x, double y) {
            AddWindowEvent(window, MousePositionEvent{.X = x, .Y = y});  //
        });

        glfwSetWindowRefreshCallback(window, [](GLFWwindow*) { NFiber::Yield(); });
    }

    void GLFW::OnMonitorEvent(GLFWmonitor* monitor, int event) {
        std::lock_guard guard(events_mutex_);
        for (auto& [_, events] : window_events_) {
            events.emplace_back(MonitorEvent{.Monitor = monitor, .Event = event});
        }
    }

}  // namespace DummyEngine
