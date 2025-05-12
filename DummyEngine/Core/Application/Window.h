#pragma once

#include "DummyEngine/Core/Application/Event.h"
#include "DummyEngine/Core/Rendering/Renderer/Context.h"
#include "DummyEngine/Utils/Debug/Logger.h"

struct GLFWWindow;

namespace DummyEngine {

    enum class WindowMode { None = 0, Windowed, FullScreen };

    struct WindowState {
        WindowMode  Mode        = WindowMode::Windowed;
        std::string Name        = "Window";
        U32         Width       = 1280;
        U32         Height      = 720;
        U32         PosX        = 100;
        U32         PosY        = 100;
        U32         MonitorID   = 0;
        bool        MouseLocked = false;
    };

    class Window {
        LOG_AUTHOR(Window)
    public:
        explicit Window(const WindowState& state = WindowState());
        ~Window();

        void OnUpdate();

        void FullScreen(U32 monitor_id);
        void Windowed(U32 width = 1280, U32 height = 720, U32 x_pos = 100, U32 y_pos = 100);

        void LockMouse();
        void UnlockMouse();
        void ToggleMouseLock();

        const WindowState& GetState() const;

    private:
        friend class ImGuiLayer;
        friend class FileSystem;

        void Invalidate();
        void SetCursorMode(U32 mode);

    private:
        WindowState    state_;
        GLFWwindow*    window_;
        Scope<Context> context_;
    };
}  // namespace DummyEngine