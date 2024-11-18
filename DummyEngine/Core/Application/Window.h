#pragma once
#include <GLFW/glfw3.h>

#include "DummyEngine/Core/Application/Event.h"
#include "DummyEngine/Core/Rendering/Renderer/Context.h"
#include "DummyEngine/Utils/Base.h"

namespace DE {

    enum class WindowMode { None = 0, Windowed, FullScreen };

    struct WindowState {
        WindowMode  mode         = WindowMode::Windowed;
        std::string name         = "Window";
        U32         width        = 1280;
        U32         height       = 720;
        U32         x_pos        = 100;
        U32         y_pos        = 100;
        U32         monitor_id   = 0;
        bool        mouse_locked = false;

        EventCallback<Event> event_callback;
    };

    class Window {
        LOGGER_AUTHOR(Window)
    public:
        Window(const WindowState& state = WindowState());
        ~Window();

        void OnUpdate();
        void SetEventCallback(EventCallback<Event> callback);

        void SetIcon(Path path);
        void FullScreen(U32 monitor_id);
        void Windowed(U32 width = 1280, U32 height = 720, U32 x_pos = 100, U32 y_pos = 100);

        void LockMouse();
        void UnlockMouse();
        void ToggleMouseLock();

        const WindowState& GetState() const;

    private:
        friend class ImGuiLayer;
        friend class FileSystem;

        void SetupCallbacks();
        void Invalidate();

        WindowState    m_State;
        GLFWwindow*    m_Window;
        Scope<Context> m_Context;
    };
}  // namespace DE