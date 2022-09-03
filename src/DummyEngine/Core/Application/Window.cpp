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

        SetupCallbacks();

        Invalidate();
    }
    Window::~Window() { glfwDestroyWindow(m_Window); }

    void Window::FullScreen(uint32_t id)
    {
        m_State.mode = WindowMode::FullScreen;

        Invalidate();
    }
    void Window::Windowed(uint32_t width, uint32_t height, uint32_t x_pos, uint32_t y_pos)
    {
        m_State.mode = WindowMode::Windowed;
        m_State.width = width;
        m_State.height = height;
        m_State.x_pos = x_pos;
        m_State.y_pos = y_pos;

        Invalidate();
    }

    void Window::LockMouse()
    {
        m_State.mouse_locked = true;
        glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    void Window::UnlockMouse()
    {
        m_State.mouse_locked = false;
        glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
    void Window::ToggleMouseLock()
    {
        if (m_State.mouse_locked)
        {
            glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
        else
        {
            glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
        m_State.mouse_locked = !m_State.mouse_locked;
    }

    void Window::OnUpdate()
    {
        glfwPollEvents();
        m_Context->SwapBuffers();
    }
    void Window::SetEventCallback(EventCallback<Event> callback) { m_State.EventCallback = callback; }

    const WindowState& Window::GetState() const { return m_State; }

    void Window::Invalidate()
    {
        DE_ASSERT(m_State.mode != WindowMode::None, "Wrong window mode.");
        DE_ASSERT(m_State.width != 0 && m_State.height != 0, "Wrong window size(" + std::to_string(m_State.width) + ", " + std::to_string(m_State.height) + ")");
        if (m_State.mode == WindowMode::Windowed)
        {
            glfwSetWindowMonitor(m_Window, nullptr, m_State.x_pos, m_State.y_pos, m_State.width, m_State.height, 200);
        }
        if (m_State.mode == WindowMode::FullScreen)
        {
            GLFWmonitor* monitor = GetMonitor(m_State.monitor_id);
            const GLFWvidmode* mode = glfwGetVideoMode(monitor);
            m_State.height = mode->height;
            m_State.width = mode->width;
            m_State.x_pos = 0;
            m_State.y_pos = 0;
            glfwSetWindowMonitor(m_Window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
        }
    }
    void Window::SetupCallbacks()
    {
        glfwSetWindowUserPointer(m_Window, &m_State);

        glfwSetWindowSizeCallback(m_Window,
                                  [](GLFWwindow* window, int width, int height)
                                  {
                                      WindowState& state = *(WindowState*)glfwGetWindowUserPointer(window);
                                      state.width = width;
                                      state.height = height;

                                      WindowResizeEvent event(width, height);
                                      state.EventCallback(event);
                                  });

        glfwSetWindowCloseCallback(m_Window,
                                   [](GLFWwindow* window)
                                   {
                                       WindowState& state = *(WindowState*)glfwGetWindowUserPointer(window);
                                       WindowCloseEvent event;
                                       state.EventCallback(event);
                                   });

        glfwSetKeyCallback(m_Window,
                           [](GLFWwindow* window, int key, int scancode, int action, int mods)
                           {
                               WindowState& state = *(WindowState*)glfwGetWindowUserPointer(window);
                               switch (action)
                               {
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

        glfwSetMouseButtonCallback(m_Window,
                                   [](GLFWwindow* window, int button, int action, int mods)
                                   {
                                       WindowState& state = *(WindowState*)glfwGetWindowUserPointer(window);

                                       switch (action)
                                       {
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

        glfwSetScrollCallback(m_Window,
                              [](GLFWwindow* window, double xOffset, double yOffset)
                              {
                                  WindowState& state = *(WindowState*)glfwGetWindowUserPointer(window);

                                  MouseScrolledEvent event((float)xOffset, (float)yOffset);
                                  state.EventCallback(event);
                              });

        glfwSetCursorPosCallback(m_Window,
                                 [](GLFWwindow* window, double xPos, double yPos)
                                 {
                                     WindowState& state = *(WindowState*)glfwGetWindowUserPointer(window);

                                     MouseMovedCallback event((float)xPos, (float)yPos);
                                     state.EventCallback(event);
                                 });
    }
}  // namespace DE