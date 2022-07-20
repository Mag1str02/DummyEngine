#include "input.h"

#include "../../../Config/config.h"

namespace DE {
void Input::IAddButton(int16_t button_id) {
    _current_frame.key_states[button_id] = glfwGetKey(_window, button_id);
}

Input::Input() {
}

void Input::ISetWindow(GLFWwindow* window) {
    _window = window;
    IReadFrame();
    IReadFrame();
}
void Input::IReadFrame() {
    _current_frame.time = glfwGetTime();
    glfwGetCursorPos(_window, &_current_frame.x_pos, &_current_frame.y_pos);
    for (size_t i = 0; i < 349; ++i) {
        IAddButton(i);
    }

    _frames.push_front(_current_frame);

    if (_frames.size() > MAX_INPUT_FRAME_AMOUNT) {
        _frames.pop_back();
    }
}

float Input::IFrameTime() const {
    return _frames[0].time - _frames[1].time;
}
double Input::ICursorXOffset() const {
    return _frames[0].x_pos - _frames[1].x_pos;
}
double Input::ICursorYOffset() const {
    return _frames[0].y_pos - _frames[1].y_pos;
}

bool Input::IKeyReleased(int16_t key_id) const {
    return !_frames[0].key_states.at(key_id) && _frames[1].key_states.at(key_id);
}
bool Input::IKeyPressed(int16_t key_id) const {
    return _frames[0].key_states.at(key_id) && !_frames[1].key_states.at(key_id);
}
bool Input::IKeyDown(int16_t key_id) const {
    return _frames[0].key_states.at(key_id);
}
bool Input::IKeyUp(int16_t key_id) const {
    return !_frames[0].key_states.at(key_id);
}

Input& Input::Get() {
    static Input input;
    return input;
}
void Input::SetWindow(GLFWwindow* window) {
    Get().ISetWindow(window);
}

void Input::ReadFrame() {
    Get().IReadFrame();
}
float Input::FrameTime() {
    return Get().IFrameTime();
}
double Input::CursorXOffset() {
    return Get().ICursorXOffset();
}
double Input::CursorYOffset() {
    return Get().ICursorYOffset();
}
            
bool Input::KeyReleased(int16_t key_id) {
    return Get().IKeyReleased(key_id);
}
bool Input::KeyPressed(int16_t key_id) {
    return Get().IKeyPressed(key_id);
}
bool Input::KeyDown(int16_t key_id) {
    return Get().IKeyDown(key_id);
}
bool Input::KeyUp(int16_t key_id) {
    return Get().IKeyUp(key_id);
}

}  // namespace DE