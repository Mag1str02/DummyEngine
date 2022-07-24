#pragma once

#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

#include "DummyEngine/Addition/Base.h"

namespace DE {

enum class WindowMode { none, windowed, borderless_windowed, fullscreen };

struct WindowState {
    std::string name;
    WindowMode window_mode;
    uint16_t width;
    uint16_t height;
};

class Window {
private:
    friend class GLRenderer;

    GLFWwindow* m_WindowHandle;
    WindowState m_State;

    GLFWmonitor* GetMonitor(uint16_t id);

public:
    Window();

    void Init();

    void FullScreen(uint16_t id);
    void BorderlessWindowed();
    void Windowed();

    void SetFrameBufferSizeCallback(void (*frame_buffer_size_callback)(GLFWwindow* window, int width, int height));

    void MakeCurrentContext() const;
    void Update(double dt);
    void SwapBuffers();
    bool ShouldClose() const;

    static void DefaultFramebufferSizeCallback(GLFWwindow* window, int width, int height);
    void SetName(std::string name);
    WindowState GetState() const;
    // Temporary func.
    GLFWwindow* GetWindow();
};
}  // namespace DE