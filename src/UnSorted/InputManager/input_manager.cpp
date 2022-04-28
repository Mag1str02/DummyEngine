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
    for (size_t i = 0; i < 349; ++i) {
        PushButton(i);
    }

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