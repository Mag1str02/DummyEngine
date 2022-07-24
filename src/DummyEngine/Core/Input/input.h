#pragma once

#include <GLFW/glfw3.h>

#include "DummyEngine/Addition/Base.h"

namespace DE {

struct InputFrame {
    double time;
    double x_pos;
    double y_pos;
    std::map<int16_t, bool> key_states;
};

class Input {
private:
    GLFWwindow* m_WindowHandle;
    InputFrame m_CurrentFrame;
    std::deque<InputFrame> m_Frames;

    void ISetWindow(GLFWwindow* window);
    void IAddButton(int16_t button_id);
    void IReadFrame();

    float IFrameTime() const;
    double ICursorXOffset() const;
    double ICursorYOffset() const;

    bool IKeyReleased(int16_t key_id) const;
    bool IKeyPressed(int16_t key_id) const;
    bool IKeyDown(int16_t key_id) const;
    bool IKeyUp(int16_t key_id) const;

    Input();

    static Input& Get();

public:
    static void SetWindow(GLFWwindow* window);

    static void ReadFrame();
    static float FrameTime();
    static double CursorXOffset();
    static double CursorYOffset();

    static bool KeyReleased(int16_t key_id);
    static bool KeyPressed(int16_t key_id);
    static bool KeyDown(int16_t key_id);
    static bool KeyUp(int16_t key_id);
};
}  // namespace DE