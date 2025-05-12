#include "Window.h"

#include "Application.h"
#include "GLFW.h"

#include "DummyEngine/Utils/Debug/Profiler.h"

#include <GLFW/glfw3.h>
#include <stb_image.h>

namespace DummyEngine {

    Window::Window(const WindowState& state) : state_(state) {
        window_ = GLFW::CreateWindow() | NFuture::Get();
        DE_ASSERT(window_, "Failed to create GLFW Window {}", state_.Name);
        LOG_INFO("Window created: {}", state_.Name);

        context_ = Context::Create(window_);
        context_->Load();

        Invalidate();
    }
    Window::~Window() {
        GLFW::DestroyWindow(window_) | NFuture::Get();
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
        SetCursorMode(GLFW_CURSOR_DISABLED);
    }
    void Window::UnlockMouse() {
        state_.MouseLocked = false;

        SetCursorMode(GLFW_CURSOR_NORMAL);
    }
    void Window::ToggleMouseLock() {
        if (state_.MouseLocked) {
            SetCursorMode(GLFW_CURSOR_NORMAL);
        } else {
            SetCursorMode(GLFW_CURSOR_DISABLED);
        }
        state_.MouseLocked = !state_.MouseLocked;
    }
    void Window::SetCursorMode(U32 mode) {
        GLFW::SetCursorMode(window_, mode) | NFuture::Get();
    }

    void Window::OnUpdate() {
        DE_PROFILE_SCOPE("Window OnUpdate");

        auto events = GLFW::PullWindowEvents(window_);
        for (const auto& event : events) {
            Application::OnEvent(event);
        }

        context_->SwapBuffers();
    }

    const WindowState& Window::GetState() const {
        return state_;
    }

    void Window::Invalidate() {
        DE_ASSERT(state_.Mode != WindowMode::None, "Wrong window mode");
        DE_ASSERT(state_.Width != 0 && state_.Height != 0, "Wrong window size ({},{}) expected non 0 sides", state_.Width, state_.Height);
        if (state_.Mode == WindowMode::Windowed) {
            NFuture::Get(GLFW::DisableFullScreen(window_));
        }
        if (state_.Mode == WindowMode::FullScreen) {
            NFuture::Get(GLFW::EnableFullScreen(window_, state_.MonitorID));
        }
    }
}  // namespace DummyEngine