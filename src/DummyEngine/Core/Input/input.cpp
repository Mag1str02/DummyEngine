#include "DummyEngine/Core/Input/Input.h"

namespace DE {
void Input::IAddButton(int16_t button_id) {
    m_CurrentFrame.key_states[button_id] = glfwGetKey(m_WindowHandle, button_id);
}

Input::Input() {
}

void Input::ISetWindow(GLFWwindow* window) {
    m_WindowHandle = window;
    IReadFrame();
    IReadFrame();
}
void Input::IReadFrame() {
    m_CurrentFrame.time = glfwGetTime();
    glfwGetCursorPos(m_WindowHandle, &m_CurrentFrame.x_pos, &m_CurrentFrame.y_pos);
    for (size_t i = 0; i < 349; ++i) {
        IAddButton(i);
    }

    m_Frames.push_front(m_CurrentFrame);

    if (m_Frames.size() > Config::GetI(DE_CFG_MAX_INPUT_FRAME_AMOUNT)) {
        m_Frames.pop_back();
    }
}

float Input::IFrameTime() const {
    return m_Frames[0].time - m_Frames[1].time;
}
double Input::ICursorXOffset() const {
    return m_Frames[0].x_pos - m_Frames[1].x_pos;
}
double Input::ICursorYOffset() const {
    return m_Frames[0].y_pos - m_Frames[1].y_pos;
}

bool Input::IKeyReleased(int16_t key_id) const {
    return !m_Frames[0].key_states.at(key_id) && m_Frames[1].key_states.at(key_id);
}
bool Input::IKeyPressed(int16_t key_id) const {
    return m_Frames[0].key_states.at(key_id) && !m_Frames[1].key_states.at(key_id);
}
bool Input::IKeyDown(int16_t key_id) const {
    return m_Frames[0].key_states.at(key_id);
}
bool Input::IKeyUp(int16_t key_id) const {
    return !m_Frames[0].key_states.at(key_id);
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