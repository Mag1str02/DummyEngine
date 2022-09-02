#pragma once

#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

#include "Core/Rendering/Renderer/Context.h"
#include "Core/Application/Event.h"
#include "Addition/Base.h"

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
        uint32_t monitor_id = 0;

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
        void Windowed();
        bool ShouldClose() const;

        const WindowState& GetState() const;

    private:
        friend class ImGuiLayer;

        void SetupCallbacks();
        void Invalidate();

        WindowState m_State;
        GLFWwindow* m_Window;
        Scope<Context> m_Context;
    };
}  // namespace DE