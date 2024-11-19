#include "DummyEngine/Core/Application/Input.h"

#include <GLFW/glfw3.h>

namespace DE {
    InputFrame::InputFrame() {
        key_states.resize((size_t)Key::Last);
    }

    SINGLETON_BASE(Input);
    S_INITIALIZE() {
        m_EventDispatcher.AddEventListener<KeyPressedEvent>([this](KeyPressedEvent& event) {
            DE_ASSERT(m_CurrentFrame.key_states.size() > event.GetKey(), "Bad key code: {}", event.GetKey());
            m_CurrentFrame.key_states[event.GetKey()] = true;
        });
        m_EventDispatcher.AddEventListener<KeyReleasedEvent>([this](KeyReleasedEvent& event) {
            DE_ASSERT(m_CurrentFrame.key_states.size() > event.GetKey(), "Bad key code: {}", event.GetKey());
            m_CurrentFrame.key_states[event.GetKey()] = false;
        });

        m_EventDispatcher.AddEventListener<SetMouseLockEvent>([this](SetMouseLockEvent&) { m_CurrentFrame.mouse_locked = true; });
        m_EventDispatcher.AddEventListener<SetMouseUnlockEvent>([this](SetMouseUnlockEvent&) { m_CurrentFrame.mouse_locked = false; });
        m_EventDispatcher.AddEventListener<SetMouseLockToggleEvent>(
            [this](SetMouseLockToggleEvent&) { m_CurrentFrame.mouse_locked = !m_CurrentFrame.mouse_locked; });

        m_EventDispatcher.AddEventListener<MouseMovedCallback>([this](MouseMovedCallback& event) {
            m_CurrentFrame.x_pos = event.GetXPos();
            m_CurrentFrame.y_pos = event.GetYPos();
        });

        INewFrame();
        return Unit();
    }
    S_TERMINATE() {
        return Unit();
    }

    S_METHOD_IMPL(Unit, OnEvent, (Event & event), (event)) {
        m_EventDispatcher.Dispatch(event);
        return Unit();
    }
    S_METHOD_IMPL(Unit, NewFrame, (), ()) {
        DE_PROFILE_SCOPE("Imput Frame Begin");

        m_Frames.push_front(m_CurrentFrame);
        if (m_Frames.size() > m_MaxFrameAmount) {
            m_Frames.pop_back();
        }
        return Unit();
    }
    S_METHOD_IMPL(Unit, SetFrameAmount, (size_t n), (n)) {
        m_MaxFrameAmount = n;
        return Unit();
    }

    S_METHOD_IMPL(double, CursorXOffset, (), ()) {
        return m_Frames[0].x_pos - m_Frames[1].x_pos;
    }
    S_METHOD_IMPL(double, CursorYOffset, (), ()) {
        return m_Frames[0].y_pos - m_Frames[1].y_pos;
    }

    S_METHOD_IMPL(bool, MouseLocked, (), ()) {
        return m_Frames[0].mouse_locked;
    }
    S_METHOD_IMPL(bool, KeyReleased, (Key key), (key)) {
        return !m_Frames[0].key_states.at((size_t)key) && m_Frames[1].key_states.at((size_t)key);
    }
    S_METHOD_IMPL(bool, KeyPressed, (Key key), (key)) {
        return m_Frames[0].key_states.at((size_t)key) && !m_Frames[1].key_states.at((size_t)key);
    }
    S_METHOD_IMPL(bool, KeyDown, (Key key), (key)) {
        return m_Frames[0].key_states.at((size_t)key);
    }
    S_METHOD_IMPL(bool, KeyUp, (Key key), (key)) {
        return !m_Frames[0].key_states.at((size_t)key);
    }

}  // namespace DE