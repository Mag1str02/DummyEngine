#include "DummyEngine/Core/Wrappings/Window.h"
#include "DummyEngine/ToolBox/Dev/FrameTimeReader.h"
#include "DummyEngine/ToolBox/Dev/Logger.h"
#include "DummyEngine/Core/ECS/SystemManager.hpp"
#include "DummyEngine/Core/Rendering/Renderer/Renderer.h"

namespace DE {

GLFWmonitor* Window::GetMonitor(uint16_t id) {
    int monitors_amount;
    GLFWmonitor** monitors = glfwGetMonitors(&monitors_amount);
    return monitors[id];
}

Window::Window() {
    m_WindowHandle = nullptr;
    m_State.height = 720;
    m_State.width = 1280;
    m_State.name = "DummyEngine";
    m_State.window_mode = WindowMode::windowed;
}
void Window::Init() {
    m_WindowHandle = glfwCreateWindow(m_State.width, m_State.height, m_State.name.c_str(), NULL, NULL);
    if (m_WindowHandle == nullptr) {
        Logger::Fatal("loading", "Window", "Failed to create GLFW Window: " + m_State.name);
        throw std::exception();
    }
    SetFrameBufferSizeCallback(DefaultFramebufferSizeCallback);
    Logger::Info("loading", "Window", "Window created: " + m_State.name);
}

void Window::FullScreen(uint16_t id) {
    GLFWmonitor* monitor = GetMonitor(id);
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    glfwSetWindowMonitor(m_WindowHandle, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
    m_State.window_mode = WindowMode::fullscreen;
}
void Window::Windowed() {
    glfwSetWindowMonitor(m_WindowHandle, nullptr, 100, 100, 1080, 720, 200);
    m_State.window_mode = WindowMode::windowed;
}

void Window::MakeCurrentContext() const {
    glfwMakeContextCurrent(m_WindowHandle);
}
void Window::SetFrameBufferSizeCallback(void (*frame_buffer_size_callback)(GLFWwindow* window, int width, int height)) {
    glfwSetFramebufferSizeCallback(m_WindowHandle, frame_buffer_size_callback);
}

void Window::DefaultFramebufferSizeCallback(GLFWwindow* window, int width, int height) {
    Renderer::OnWindowResize(width, height);
}
void Window::Update(double dt) {
}
void Window::SwapBuffers() {
    glfwSwapBuffers(m_WindowHandle);
}
bool Window::ShouldClose() const {
    return glfwWindowShouldClose(m_WindowHandle);
}

void Window::SetName(std::string name) {
    m_State.name = name;
    if (m_WindowHandle) {
        glfwSetWindowTitle(m_WindowHandle, name.c_str());
    }
}
WindowState Window::GetState() const {
    return m_State;
}
GLFWwindow* Window::GetWindow() {
    return m_WindowHandle;
}
}  // namespace DE
   /*
   void Window::StartLoop() {
       double avarage_frame_time = 0;
       double prev_time;
       size_t frame_amount = 0;
       double max_frame_time = 0;
       while (!glfwWindowShouldClose(m_Window)) {
           double prev_frame_time = glfwGetTime() - prev_time;
           prev_time = glfwGetTime();
           DE_FTR_ENTER("");
           DE_FTR_ENTER("Poll Events");
           glfwPollEvents();
           DE_FTR_LEAVE();
           DE_FTR_ENTER("Process Input");
           _process_input();
           DE_FTR_LEAVE();
           DE_FTR_ENTER("System Processing");
           SystemManager::Update(prev_frame_time);
           DE_FTR_LEAVE();
           DE_FTR_ENTER("Frame Swap");
           glfwSwapBuffers(m_Window);
           max_frame_time = std::max(max_frame_time, glfwGetTime() - prev_time);
           avarage_frame_time = (avarage_frame_time * frame_amount + glfwGetTime() - prev_time) / (frame_amount + 1);
           frame_amount++;
           DE_FTR_LEAVE();
           DE_FTR_LEAVE();
           DE_FTR_PRINT();
       }
       Logger::Info("rendering", "Window", "Avarage frame time: " + std::to_string(avarage_frame_time) + "s" + " FPS: " + std::to_string(1.0 / avarage_frame_time));
       Logger::Info("rendering", "Window", "Avarage frame time: " + std::to_string(max_frame_time) + "s" + " FPS: " + std::to_string(1.0 / max_frame_time));
   }
   
   void Window::MakeCurrentContext() {
       glfwMakeContextCurrent(m_Window);
   }
   */