#include "window.h"

#include "../../../ToolBox/Dev/FrameTimeReader/frame_time_reader.h"
#include "../../../ToolBox/Dev/Logger/logger.h"
#include "../../ECS/System/system_manager.hpp"

namespace DE {

GLFWmonitor* Window::GetMonitor(int id) {
    int monitors_amount;
    GLFWmonitor** monitors = glfwGetMonitors(&monitors_amount);
    return monitors[id];
}

Window::Window() {
    _window = nullptr;
    _state.height = 720;
    _state.width = 1280;
    _state.name = "DummyEngine";
    _state.screen_size_state = windowed;
}

void Window::Init() {
    _window = glfwCreateWindow(_state.height, _state.width, name.c_str(), NULL, NULL);

    if (_window == NULL) {
        Logger::Fatal("loading", "Window", "Failed to create GLFW Window: (" + _name + ")");
        throw std::exception();
    }
    Logger::Info("loading", "Window", "GLFW Window created: (" + _name + ")");
}

void Window::FullScreen(int id) {
    GLFWmonitor* monitor = GetMonitor(id);
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    glfwSetWindowMonitor(_window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
    _screen_size_state = ScreenSizeState::fullscreen;
}
void Window::Windowed() {
    glfwSetWindowMonitor(_window, nullptr, 100, 100, 1080, 720, 200);
    _screen_size_state = ScreenSizeState::windowed;
}

void Window::SetFrameBufferSizeCallback(void (*frame_buffer_size_callback)(GLFWwindow* window, int width, int height)) {
    glfwSetFramebufferSizeCallback(_window, frame_buffer_size_callback);
}

WindowStates Window::GetScreenSizeState() const {
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
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        Logger::Fatal("loading", "Window", "Failed to initialize GLAD");
        throw std::exception();
    }
    Logger::Info("loading", "Window", "GLAD Initialized");
}
*/