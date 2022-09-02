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

    void Input::IOnEvent(Event& event)
    {
        EventDispatcher dispatcher;
        dispatcher.AddEventListener<KeyPressedEvent>([this](KeyPressedEvent& event) { m_CurrentFrame.key_states[event.GetKey()] = true; });
        dispatcher.AddEventListener<KeyReleasedEvent>([this](KeyReleasedEvent& event) { m_CurrentFrame.key_states[event.GetKey()] = false; });
    }
    void Input::INewFrame()
    {
        m_Frames.push_front(m_CurrentFrame);
    }

    double Input::ICursorXOffset() const
    {
        return m_Frames[0].x_pos - m_Frames[1].x_pos;
    }
    double Input::ICursorYOffset() const
    {
        return m_Frames[0].y_pos - m_Frames[1].y_pos;
    }
    bool Input::IKeyReleased(int16_t key_id) const
    {
        return !m_Frames[0].key_states.at(key_id) && m_Frames[1].key_states.at(key_id);
    }
    bool Input::IKeyPressed(int16_t key_id) const
    {
        return m_Frames[0].key_states.at(key_id) && !m_Frames[1].key_states.at(key_id);
    }
    bool Input::IKeyDown(int16_t key_id) const
    {
        return m_Frames[0].key_states.at(key_id);
    }
    bool Input::IKeyUp(int16_t key_id) const
    {
        return !m_Frames[0].key_states.at(key_id);
    }

    Input& Input::Get()
    {
        static Input input;
        return input;
    }

    void Input::OnEvent(Event& event)
    {
        Get().IOnEvent(event);
    }
    void Input::NewFrame()
    {
        Get().INewFrame();
    }

    double Input::CursorXOffset()
    {
        return Get().ICursorXOffset();
    }
    double Input::CursorYOffset()
    {
        return Get().ICursorYOffset();
    }
    bool Input::KeyReleased(int16_t key_id)
    {
        return Get().IKeyReleased(key_id);
    }
    bool Input::KeyPressed(int16_t key_id)
    {
        return Get().IKeyPressed(key_id);
    }
    bool Input::KeyDown(int16_t key_id)
    {
        return Get().IKeyDown(key_id);
    }
    bool Input::KeyUp(int16_t key_id)
    {
        return Get().IKeyUp(key_id);
    }

}  // namespace DE