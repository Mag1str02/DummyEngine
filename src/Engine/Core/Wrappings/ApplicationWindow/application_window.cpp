#include "application_window.h"

#include "../../../ToolBox/Dev/Logger/logger.h"
namespace DE {
void ApplicationWindow::CheckWindow() {
    if (_window == NULL) {
        Logger::Fatal("loading", "ApplicationWindow", "Failed to create GLFW Window: " + _name);
        throw std::exception();
    }
    Logger::Info("loading", "ApplicationWindow", "GLFW Window created: " + _name);
}
void ApplicationWindow::MakeCurrentContext() {
    glfwMakeContextCurrent(_window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        Logger::Fatal("loading", "ApplicationWindow", "Failed to initialize GLAD");
        throw std::exception();
    }
    Logger::Info("loading", "ApplicationWindow", "GLAD Initialized");
}

GLFWmonitor* ApplicationWindow::GetMonitor(int id) {
    int monitors_amount;
    GLFWmonitor** monitors = glfwGetMonitors(&monitors_amount);
    return monitors[id];
}
void ApplicationWindow::DefaultFrameBufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

ApplicationWindow::ApplicationWindow() {
}

void ApplicationWindow::Init(std::string name) {
    _name = name;
    _window = glfwCreateWindow(1080, 720, name.c_str(), NULL, NULL);
    _screen_size_state = ScreenSizeState::windowed;
    CheckWindow();
    MakeCurrentContext();
    glfwSetFramebufferSizeCallback(_window, DefaultFrameBufferSizeCallback);
    glfwSwapInterval(0);
    SetFullScreen(0);
    glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    Logger::Info("loading", "ApplicationWindow", "Application Window initialized: " + _name);
}
void ApplicationWindow::SetFullScreen(int id) {
    GLFWmonitor* monitor = GetMonitor(id);
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    glfwSetWindowMonitor(_window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
    _screen_size_state = ScreenSizeState::fullscreen;
}
void ApplicationWindow::SetWindowed() {
    glfwSetWindowMonitor(_window, nullptr, 100, 100, 1080, 720, 200);
    _screen_size_state = ScreenSizeState::windowed;
}

void ApplicationWindow::StartLoop() {
    double avarage_frame_time = 0;
    double prev_time;
    size_t frame_amount = 0;
    while (!glfwWindowShouldClose(_window)) {
        prev_time = glfwGetTime();
        glfwPollEvents();
        _process_input();
        _update_world_func();
        _draw_func();
        glfwSwapBuffers(_window);
        avarage_frame_time = (avarage_frame_time * frame_amount + glfwGetTime() - prev_time) / (frame_amount + 1);
        frame_amount++;
    }
    Logger::Info("rendering", "ApplicationWindow",
                 "Avarage frame time: " + std::to_string(avarage_frame_time) + "s" + " FPS: " + std::to_string(1.0 / avarage_frame_time));
}

void ApplicationWindow::SetDrawFunc(void (*draw_func)()) {
    _draw_func = draw_func;
}
void ApplicationWindow::SetUpdateWorldFunc(void (*update_world_func)()) {
    _update_world_func = update_world_func;
}
void ApplicationWindow::SetProcessInputFunc(void (*process_input)()) {
    _process_input = process_input;
}
void ApplicationWindow::SetFrameBufferSizeCallback(void (*frame_buffer_size_callback)(GLFWwindow* window, int width, int height)) {
    glfwSetFramebufferSizeCallback(_window, frame_buffer_size_callback);
}

ScreenSizeState ApplicationWindow::GetScreenSizeState() const {
    return _screen_size_state;
}
GLFWwindow* ApplicationWindow::GetWindow() {
    return _window;
}
}  // namespace DE