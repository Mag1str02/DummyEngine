#include "Core/Application/Window.h"
#include "ToolBox/Dev/FrameTimeReader.h"
#include "ToolBox/Dev/Logger.h"
#include "Core/ECS/SystemManager.hpp"
#include "Core/Rendering/Renderer/Renderer.h"

namespace DE
{

    GLFWmonitor* GetMonitor(uint32_t id)
    {
        int monitors_amount;
        GLFWmonitor** monitors = glfwGetMonitors(&monitors_amount);
        DE_ASSERT(0 <= id && id < monitors_amount, "Wrong monitor id.");
        return monitors[id];
    }

    Window::Window(const WindowState& state) : m_State(state)
    {
        m_Window = glfwCreateWindow(1280, 720, m_State.name.c_str(), NULL, NULL);
        DE_ASSERT(m_Window != nullptr, "Failed to create GLFW Window:" + m_State.name);
        Logger::Info("loading", "Window", "Window created: " + m_State.name);

        m_Context = Context::Create(m_Window);
        m_Context->Load();

        Invalidate();
    }
    Window::~Window()
    {
        glfwDestroyWindow(m_Window);
    }

    void Window::FullScreen(uint32_t id)
    {
        m_State.mode = WindowMode::FullScreen;

        Invalidate();
    }
    void Window::Windowed()
    {
        m_State.mode = WindowMode::Windowed;

        Invalidate();
    }
    bool Window::ShouldClose() const
    {
        return glfwWindowShouldClose(m_Window);
    }

    void Window::OnUpdate()
    {
        glfwPollEvents();
        m_Context->SwapBuffers();
    }

    const WindowState& Window::GetState() const
    {
        return m_State;
    }

    void Window::Invalidate()
    {
        DE_ASSERT(m_State.mode != WindowMode::None, "Wrong window mode.");
        DE_ASSERT(m_State.width != 0 && m_State.height != 0,
                  "Wrong window size(" + std::to_string(m_State.width) + ", " + std::to_string(m_State.height) + ")");
        if (m_State.mode == WindowMode::Windowed)
        {
            glfwSetWindowMonitor(m_Window, nullptr, 100, 100, m_State.width, m_State.height, 200);
        }
        if (m_State.mode == WindowMode::FullScreen)
        {
            GLFWmonitor* monitor = GetMonitor(m_State.monitor_id);
            const GLFWvidmode* mode = glfwGetVideoMode(monitor);
            m_State.height = mode->height;
            m_State.width = mode->width;
            glfwSetWindowMonitor(m_Window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
        }
    }
}  // namespace DE