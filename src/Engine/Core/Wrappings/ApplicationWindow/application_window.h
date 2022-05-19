#pragma once

#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

#include <string>
namespace DE {
enum ScreenSizeState { none, windowed, borderless_windowed, fullscreen };

class ApplicationWindow {
private:
    GLFWwindow* _window;
    ScreenSizeState _screen_size_state;
    std::string _name;

    void (*_process_input)();

    void CheckWindow();

    GLFWmonitor* GetMonitor(int id);

    static void DefaultFrameBufferSizeCallback(GLFWwindow* window, int width, int height);

public:
    ApplicationWindow();
    void MakeCurrentContext();

    void Init(std::string name);
    void SetFullScreen(int id);
    void SetWindowed();

    void StartLoop();

    void SetProcessInputFunc(void (*process_input)());
    void SetFrameBufferSizeCallback(void (*frame_buffer_size_callback)(GLFWwindow* window, int width, int height));

    ScreenSizeState GetScreenSizeState() const;
    GLFWwindow* GetWindow();
};
}  // namespace DE