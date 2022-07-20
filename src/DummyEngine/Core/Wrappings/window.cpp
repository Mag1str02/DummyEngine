#include "DummyEngine/Core/Wrappings/window.h"
#include "DummyEngine/ToolBox/Dev/frame_time_reader.h"
#include "DummyEngine/ToolBox/Dev/logger.h"
#include "DummyEngine/Core/ECS/system_manager.hpp"
#include "DummyEngine/Core/Rendering/RendererOpenGL/gl_renderer.h"

namespace DE {

GLFWmonitor* Window::GetMonitor(uint16_t id) {
    int monitors_amount;
    GLFWmonitor** monitors = glfwGetMonitors(&monitors_amount);
    return monitors[id];
}

Window::Window() {
    _window = nullptr;
    _state.height = 720;
    _state.width = 1280;
    _state.name = "DummyEngine";
    _state.window_mode = WindowMode::windowed;
}
void Window::Init() {
    _window = glfwCreateWindow(_state.width, _state.height, _state.name.c_str(), NULL, NULL);
    if (_window == nullptr) {
        Logger::Fatal("loading", "Window", "Failed to create GLFW Window: " + _state.name);
        throw std::exception();
    }
    SetFrameBufferSizeCallback(DefaultFramebufferSizeCallback);
    Logger::Info("loading", "Window", "Window created: " + _state.name);
}

void Window::FullScreen(uint16_t id) {
    GLFWmonitor* monitor = GetMonitor(id);
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    glfwSetWindowMonitor(_window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
    _state.window_mode = WindowMode::fullscreen;
}
void Window::Windowed() {
    glfwSetWindowMonitor(_window, nullptr, 100, 100, 1080, 720, 200);
    _state.window_mode = WindowMode::windowed;
}

void Window::MakeCurrentContext() const {
    glfwMakeContextCurrent(_window);
}
void Window::SetFrameBufferSizeCallback(void (*frame_buffer_size_callback)(GLFWwindow* window, int width, int height)) {
    glfwSetFramebufferSizeCallback(_window, frame_buffer_size_callback);
}

void Window::DefaultFramebufferSizeCallback(GLFWwindow* window, int width, int height) {
    GLRenderer::SetViewport(width, height, 0, 0);
}
void Window::Update(double dt) {
}
void Window::SwapBuffers() {
    glfwSwapBuffers(_window);
}
bool Window::ShouldClose() const {
    return glfwWindowShouldClose(_window);
}

void Window::SetName(std::string name) {
    _state.name = name;
    if (_window) {
        glfwSetWindowTitle(_window, name.c_str());
    }
}
WindowState Window::GetState() const {
    return _state;
}
GLFWwindow* Window::GetWindow() {
    return _window;
}
}  // namespace DE
   /*
   void Window::StartLoop() {
       double avarage_frame_time = 0;
       double prev_time;
       size_t frame_amount = 0;
       double max_frame_time = 0;
       while (!glfwWindowShouldClose(_window)) {
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
           glfwSwapBuffers(_window);
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
       glfwMakeContextCurrent(_window);
   }
   */