#include "input_manager.h"
#include "../Config/config.h"

void InputManager::PushButton(int button_id) {
    _current_frame.key_states[button_id] = glfwGetKey(_window, button_id);
}

InputManager::InputManager() {
}

void InputManager::SetWindow(GLFWwindow* window) {
    _window = window;
    ReadFrame();
    ReadFrame();
}
void InputManager::ReadFrame() {
    _current_frame.time = glfwGetTime();
    glfwGetCursorPos(_window, &_current_frame.x_pos, &_current_frame.y_pos);

    PushButton(GLFW_KEY_ESCAPE);

    PushButton(GLFW_KEY_LEFT_SHIFT);
    PushButton(GLFW_KEY_TAB);
    PushButton(GLFW_KEY_F11);
    PushButton(GLFW_KEY_0);
    PushButton(GLFW_KEY_1);

    PushButton(GLFW_KEY_S);
    PushButton(GLFW_KEY_W);
    PushButton(GLFW_KEY_D);
    PushButton(GLFW_KEY_A);
    PushButton(GLFW_KEY_Q);
    PushButton(GLFW_KEY_E);
    PushButton(GLFW_KEY_SPACE);
    PushButton(GLFW_KEY_C);

    _frames.push_front(_current_frame);

    if (_frames.size() > MAX_INPUT_FRAME_AMOUNT) {
        _frames.pop_back();
    }
}

float InputManager::FrameTime() const {
    return _frames[0].time - _frames[1].time;
}
double InputManager::CursorXOffset() const {
    return _frames[0].x_pos - _frames[1].x_pos;
}
double InputManager::CursorYOffset() const {
    return _frames[0].y_pos - _frames[1].y_pos;
}

bool InputManager::KeyReleased(int key_id) const {
    return !_frames[0].key_states.at(key_id) && _frames[1].key_states.at(key_id);
}
bool InputManager::KeyPressed(int key_id) const {
    return _frames[0].key_states.at(key_id) && !_frames[1].key_states.at(key_id);
}
bool InputManager::KeyDown(int key_id) const {
    return _frames[0].key_states.at(key_id);
}
bool InputManager::KeyUp(int key_id) const {
    return !_frames[0].key_states.at(key_id);
}