#pragma once

#include <GLFW/glfw3.h>

#include <deque>
#include <map>

struct InputFrame {
    double time;
    double x_pos;
    double y_pos;
    std::map<int, bool> key_states;
};

class InputManager {
private:
    GLFWwindow* _window;
    InputFrame _current_frame;
    std::deque<InputFrame> _frames;

    void PushButton(int button_id);

public:
    InputManager();

    void SetWindow(GLFWwindow* window);
    void ReadFrame();

    float FrameTime() const;
    double CursorXOffset() const;
    double CursorYOffset() const;

    bool KeyReleased(int key_id) const;
    bool KeyPressed(int key_id) const;
    bool KeyDown(int key_id) const;
    bool KeyUp(int key_id) const;
};