#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "DummyEngine/Core/Rendering/Renderer/Context.h"
#include "DummyEngine/Core/Application/Event.h"
#include "DummyEngine/Addition/Base.h"

namespace DE
{

    enum class WindowMode
    {
        None = 0,
        Windowed,
        FullScreen
    };

    struct WindowState
    {
        WindowMode mode = WindowMode::Windowed;
        std::string name = "Window";
        uint32_t width = 1280;
        uint32_t height = 720;
        uint32_t x_pos = 100;
        uint32_t y_pos = 100;
        uint32_t monitor_id = 0;
        bool mouse_locked = false;

        EventCallback<Event> EventCallback;
    };

    class Window
    {
    public:
        Window(const WindowState& state = WindowState());
        ~Window();

        void OnUpdate();
        void SetEventCallback(EventCallback<Event> callback);

        void FullScreen(uint32_t monitor_id);
        void Windowed(uint32_t width = 1280, uint32_t height = 720, uint32_t x_pos = 100, uint32_t y_pos = 100);

        void LockMouse();
        void UnlockMouse();
        void ToggleMouseLock();

        const WindowState& GetState() const;

    private:
        friend class ImGuiLayer;
        friend class FileDialogs;

        void SetupCallbacks();
        void Invalidate();

        WindowState m_State;
        GLFWwindow* m_Window;
        Scope<Context> m_Context;
    };
}  // namespace DE