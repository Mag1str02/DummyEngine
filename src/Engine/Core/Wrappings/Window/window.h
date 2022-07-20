#pragma once

#include <GLFW/glfw3.h>

#include <string>

namespace DE {
enum ScreenSizeState { none, windowed, borderless_windowed, fullscreen };

class Window {
private:
    struct WindowState {
        std::string name;
        ScreenSizeState screen_size_state;
        uint16_t width;
        uint16_t height;
    }

    GLFWwindow* _window;
    WindowState _state;

    GLFWmonitor* GetMonitor(int id);

    static void DefaultFrameBufferSizeCallback(GLFWwindow* window, int width, int height);

public: 
    Window();

    void Init(std::string name);

    void FullScreen(int id);
    void BorderlessWindowed();
    void Windowed();

    void SetFrameBufferSizeCallback(void (*frame_buffer_size_callback)(GLFWwindow* window, int width, int height));

    WindowState GetWindowState() const;
    GLFWwindow* GetWindow();
};
}  // namespace DE