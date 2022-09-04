#include <GLFW/glfw3.h>

#include "Core/Application/Input.h"

namespace DE
{

    InputFrame::InputFrame()
    {
        for (size_t i = 0; i < GLFW_KEY_LAST; ++i)
        {
            key_states[i] = false;
        }
    }

    Input::Input()
    {
        m_EventDispatcher.AddEventListener<KeyPressedEvent>([this](KeyPressedEvent& event) { m_CurrentFrame.key_states[event.GetKey()] = true; });
        m_EventDispatcher.AddEventListener<KeyReleasedEvent>([this](KeyReleasedEvent& event) { m_CurrentFrame.key_states[event.GetKey()] = false; });

        m_EventDispatcher.AddEventListener<SetMouseLockEvent>([this](SetMouseLockEvent& event) { m_CurrentFrame.mouse_locked = true; });
        m_EventDispatcher.AddEventListener<SetMouseUnlockEvent>([this](SetMouseUnlockEvent& event) { m_CurrentFrame.mouse_locked = false; });
        m_EventDispatcher.AddEventListener<SetMouseLockToggleEvent>([this](SetMouseLockToggleEvent& event) { m_CurrentFrame.mouse_locked = !m_CurrentFrame.mouse_locked; });

        m_EventDispatcher.AddEventListener<MouseMovedCallback>(
            [this](MouseMovedCallback& event)
            {
                m_CurrentFrame.x_pos = event.GetXPos();
                m_CurrentFrame.y_pos = event.GetYPos();
            });
        INewFrame();
    }

    void Input::IOnEvent(Event& event) { m_EventDispatcher.Dispatch(event); }
    void Input::INewFrame() { m_Frames.push_front(m_CurrentFrame); }

    double Input::ICursorXOffset() const { return m_Frames[0].x_pos - m_Frames[1].x_pos; }
    double Input::ICursorYOffset() const { return m_Frames[0].y_pos - m_Frames[1].y_pos; }

    bool Input::IMouseLocked() const { return m_Frames[0].mouse_locked; }
    bool Input::IKeyReleased(uint32_t key_id) const { return !m_Frames[0].key_states.at(key_id) && m_Frames[1].key_states.at(key_id); }
    bool Input::IKeyPressed(uint32_t key_id) const { return m_Frames[0].key_states.at(key_id) && !m_Frames[1].key_states.at(key_id); }
    bool Input::IKeyDown(uint32_t key_id) const { return m_Frames[0].key_states.at(key_id); }
    bool Input::IKeyUp(uint32_t key_id) const { return !m_Frames[0].key_states.at(key_id); }

    Input& Input::Get()
    {
        static Input input;
        return input;
    }

    void Input::OnEvent(Event& event) { Get().IOnEvent(event); }
    void Input::NewFrame()
    {
        DE_PROFILE_SCOPE("Imput Frame Begin");

        Get().INewFrame();
    }

    double Input::CursorXOffset() { return Get().ICursorXOffset(); }
    double Input::CursorYOffset() { return Get().ICursorYOffset(); }

    bool Input::MouseLocked() { return Get().IMouseLocked(); }

    bool Input::KeyReleased(uint32_t key_id) { return Get().IKeyReleased(key_id); }
    bool Input::KeyPressed(uint32_t key_id) { return Get().IKeyPressed(key_id); }
    bool Input::KeyDown(uint32_t key_id) { return Get().IKeyDown(key_id); }
    bool Input::KeyUp(uint32_t key_id) { return Get().IKeyUp(key_id); }

}  // namespace DE