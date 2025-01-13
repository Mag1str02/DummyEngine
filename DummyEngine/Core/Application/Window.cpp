#include "Window.h"

#include "DummyEngine/Utils/Debug/Profiler.h"

#include <GLFW/glfw3.h>
#include <stb_image.h>

namespace DummyEngine {

    GLFWmonitor* GetMonitor(U32 id) {
        int           monitors_amount = 0;
        GLFWmonitor** monitors        = glfwGetMonitors(&monitors_amount);
        DE_ASSERT(id < U32(monitors_amount), "Wrong monitor id {} should be between [0, {})", id, monitors_amount);
        return monitors[id];
    }

    Window::Window(const WindowState& state) : state_(state) {
        window_ = glfwCreateWindow(1280, 720, state_.Name.c_str(), nullptr, nullptr);
        DE_ASSERT(window_, "Failed to create GLFW Window {}", state_.Name);
        LOG_INFO("Window created: {}", state_.Name);

        context_ = Context::Create(window_);
        context_->Load();

        SetupCallbacks();

        Invalidate();
    }
    Window::~Window() {
        glfwDestroyWindow(window_);
    }

    void Window::SetIcon(Path path) {
        GLFWimage icon;
        icon.pixels = stbi_load(path.string().c_str(), &icon.width, &icon.height, nullptr, 4);

        if (icon.pixels == nullptr) {
            LOG_WARNING("Failed to set window icon {}", path);
            return;
        }
        glfwSetWindowIcon(window_, 1, &icon);
        stbi_image_free(icon.pixels);
    }
    void Window::FullScreen(U32 id) {
        state_.Mode      = WindowMode::FullScreen;
        state_.MonitorID = id;

        Invalidate();
    }
    void Window::Windowed(U32 width, U32 height, U32 x_pos, U32 y_pos) {
        state_.Mode   = WindowMode::Windowed;
        state_.Width  = width;
        state_.Height = height;
        state_.PosX   = x_pos;
        state_.PosY   = y_pos;

        Invalidate();
    }

    void Window::LockMouse() {
        state_.MouseLocked = true;
        glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    void Window::UnlockMouse() {
        state_.MouseLocked = false;
        glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
    void Window::ToggleMouseLock() {
        if (state_.MouseLocked) {
            glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        } else {
            glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
        state_.MouseLocked = !state_.MouseLocked;
    }

    void Window::OnUpdate() {
        DE_PROFILE_SCOPE("Window OnUpdate");

        glfwPollEvents();
        context_->SwapBuffers();
    }
    void Window::SetEventCallback(EventCallback<Event> callback) {
        state_.EventCallback = callback;
    }

    const WindowState& Window::GetState() const {
        return state_;
    }

    void Window::Invalidate() {
        DE_ASSERT(state_.Mode != WindowMode::None, "Wrong window mode");
        DE_ASSERT(state_.Width != 0 && state_.Height != 0, "Wrong window size ({},{}) expected non 0 sides", state_.Width, state_.Height);
        if (state_.Mode == WindowMode::Windowed) {
            glfwSetWindowMonitor(window_, nullptr, state_.PosX, state_.PosY, state_.Width, state_.Height, 1000);
        }
        if (state_.Mode == WindowMode::FullScreen) {
            GLFWmonitor*       monitor = GetMonitor(state_.MonitorID);
            const GLFWvidmode* mode    = glfwGetVideoMode(monitor);
            state_.Height              = mode->height;
            state_.Width               = mode->width;
            state_.PosX                = 0;
            state_.PosY                = 0;
            glfwSetWindowMonitor(window_, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
        }
    }
    void Window::SetupCallbacks() {
        glfwSetWindowUserPointer(window_, &state_);

        glfwSetWindowSizeCallback(window_, [](GLFWwindow* window, int width, int height) {
            WindowState& state = *(WindowState*)glfwGetWindowUserPointer(window);
            state.Width        = width;
            state.Height       = height;

            WindowResizeEvent event(width, height);
            state.EventCallback(event);
        });

        glfwSetWindowCloseCallback(window_, [](GLFWwindow* window) {
            WindowState&     state = *(WindowState*)glfwGetWindowUserPointer(window);
            WindowCloseEvent event;
            state.EventCallback(event);
        });

        glfwSetKeyCallback(window_, [](GLFWwindow* window, int key, int, int action, int) {
            if (key == -1) {
                return;
            }
            WindowState& state = *(WindowState*)glfwGetWindowUserPointer(window);
            switch (action) {
                case GLFW_PRESS: {
                    KeyPressedEvent event(key);
                    state.EventCallback(event);
                    break;
                }
                case GLFW_RELEASE: {
                    KeyReleasedEvent event(key);
                    state.EventCallback(event);
                    break;
                }
                case GLFW_REPEAT: {
                    KeyPressedEvent event(key);
                    state.EventCallback(event);
                    break;
                }
            }
        });

        glfwSetMouseButtonCallback(window_, [](GLFWwindow* window, int button, int action, int) {
            WindowState& state = *(WindowState*)glfwGetWindowUserPointer(window);
            switch (action) {
                case GLFW_PRESS: {
                    MouseButtonPressedEvent event(button);
                    state.EventCallback(event);
                    break;
                }
                case GLFW_RELEASE: {
                    MouseButtonReleasedEvent event(button);
                    state.EventCallback(event);
                    break;
                }
            }
        });

        glfwSetScrollCallback(window_, [](GLFWwindow* window, double x_offset, double y_offset) {
            WindowState& state = *(WindowState*)glfwGetWindowUserPointer(window);

            MouseScrolledEvent event((float)x_offset, (float)y_offset);
            state.EventCallback(event);
        });

        glfwSetCursorPosCallback(window_, [](GLFWwindow* window, double x_pos, double y_pos) {
            WindowState& state = *(WindowState*)glfwGetWindowUserPointer(window);

            MouseMovedCallback event((float)x_pos, (float)y_pos);
            state.EventCallback(event);
        });
    }
}  // namespace DummyEngine