#include "Core/Wrappings/Window.h"
#include "ToolBox/Dev/FrameTimeReader.h"
#include "ToolBox/Dev/Logger.h"
#include "Core/ECS/SystemManager.hpp"
#include "Core/Rendering/Renderer/Renderer.h"

namespace DE
{

    GLFWmonitor* Window::GetMonitor(uint16_t id)
    {
        int monitors_amount;
        GLFWmonitor** monitors = glfwGetMonitors(&monitors_amount);
        return monitors[id];
    }

    Window::Window()
    {
        m_WindowHandle = nullptr;
        m_State.height = 720;
        m_State.width = 1280;
        m_State.name = "DummyEngine";
        m_State.window_mode = WindowMode::windowed;
    }
    void Window::Init()
    {
        m_WindowHandle = glfwCreateWindow(m_State.width, m_State.height, m_State.name.c_str(), NULL, NULL);
        DE_ASSERT(m_WindowHandle != nullptr, "Failed to create GLFW Window:" + m_State.name);
        SetFrameBufferSizeCallback(DefaultFramebufferSizeCallback);
        Logger::Info("loading", "Window", "Window created: " + m_State.name);
    }

    void Window::FullScreen(uint16_t id)
    {
        GLFWmonitor* monitor = GetMonitor(id);
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        glfwSetWindowMonitor(m_WindowHandle, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
        m_State.window_mode = WindowMode::fullscreen;
    }
    void Window::Windowed()
    {
        glfwSetWindowMonitor(m_WindowHandle, nullptr, 100, 100, 1080, 720, 200);
        m_State.window_mode = WindowMode::windowed;
    }

    void Window::MakeCurrentContext() const
    {
        glfwMakeContextCurrent(m_WindowHandle);
    }
    void Window::SetFrameBufferSizeCallback(void (*frame_buffer_size_callback)(GLFWwindow* window,
                                                                               int width,
                                                                               int height))
    {
        glfwSetFramebufferSizeCallback(m_WindowHandle, frame_buffer_size_callback);
    }

    void Window::DefaultFramebufferSizeCallback(GLFWwindow* window, int width, int height)
    {
        Renderer::OnWindowResize(width, height);
    }
    void Window::Update(double dt) {}
    void Window::SwapBuffers()
    {
        glfwSwapBuffers(m_WindowHandle);
    }
    bool Window::ShouldClose() const
    {
        return glfwWindowShouldClose(m_WindowHandle);
    }

    void Window::SetName(std::string name)
    {
        m_State.name = name;
        if (m_WindowHandle)
        {
            glfwSetWindowTitle(m_WindowHandle, name.c_str());
        }
    }
    WindowState Window::GetState() const
    {
        return m_State;
    }
    GLFWwindow* Window::GetWindow()
    {
        return m_WindowHandle;
    }
}  // namespace DE