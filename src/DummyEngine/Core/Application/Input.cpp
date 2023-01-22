#include <GLFW/glfw3.h>
#include "DummyEngine/Core/Application/Input.h"

namespace DE
{
    InputFrame::InputFrame() { key_states.resize(KEY_LAST); }

    SINGLETON_BASE(Input);

    Unit Input::Initialize()
    {
        DE_ASSERT(!s_Instance, "Double Input initialization");
        s_Instance = new Input();
        DE_ASSERT(s_Instance, "Failed to allocate memory for Input");
        s_Instance->IInitialize();
        return Unit();
    }
    Unit Input::IInitialize()
    {
        m_EventDispatcher.AddEventListener<KeyPressedEvent>([this](KeyPressedEvent& event) { m_CurrentFrame.key_states[event.GetKey()] = true; });
        m_EventDispatcher.AddEventListener<KeyReleasedEvent>([this](KeyReleasedEvent& event) { m_CurrentFrame.key_states[event.GetKey()] = false; });

        m_EventDispatcher.AddEventListener<SetMouseLockEvent>([this](SetMouseLockEvent& event) { m_CurrentFrame.mouse_locked = true; });
        m_EventDispatcher.AddEventListener<SetMouseUnlockEvent>([this](SetMouseUnlockEvent& event) { m_CurrentFrame.mouse_locked = false; });
        m_EventDispatcher.AddEventListener<SetMouseLockToggleEvent>([this](SetMouseLockToggleEvent& event)
                                                                    { m_CurrentFrame.mouse_locked = !m_CurrentFrame.mouse_locked; });

        m_EventDispatcher.AddEventListener<MouseMovedCallback>(
            [this](MouseMovedCallback& event)
            {
                m_CurrentFrame.x_pos = event.GetXPos();
                m_CurrentFrame.y_pos = event.GetYPos();
            });

        INewFrame();
        return Unit();
    }
    Unit Input::Terminate()
    {
        s_Instance->ITerminate();
        delete s_Instance;
        return Unit();
    }
    Unit Input::ITerminate() { return Unit(); }

    S_METHOD_IMPL(Input, Unit, OnEvent, (Event & event), (event))
    {
        m_EventDispatcher.Dispatch(event);
        return Unit();
    }
    S_METHOD_IMPL(Input, Unit, NewFrame, (), ())
    {
        DE_PROFILE_SCOPE("Imput Frame Begin");

        m_Frames.push_front(m_CurrentFrame);
        return Unit();
    }

    S_METHOD_IMPL(Input, double, CursorXOffset, (), ()) { return m_Frames[0].x_pos - m_Frames[1].x_pos; }
    S_METHOD_IMPL(Input, double, CursorYOffset, (), ()) { return m_Frames[0].y_pos - m_Frames[1].y_pos; }

    S_METHOD_IMPL(Input, bool, MouseLocked, (), ()) { return m_Frames[0].mouse_locked; }
    S_METHOD_IMPL(Input, bool, KeyReleased, (uint32_t key_id), (key_id))
    {
        return !m_Frames[0].key_states.at(key_id) && m_Frames[1].key_states.at(key_id);
    }
    S_METHOD_IMPL(Input, bool, KeyPressed, (uint32_t key_id), (key_id))
    {
        return m_Frames[0].key_states.at(key_id) && !m_Frames[1].key_states.at(key_id);
    }
    S_METHOD_IMPL(Input, bool, KeyDown, (uint32_t key_id), (key_id)) { return m_Frames[0].key_states.at(key_id); }
    S_METHOD_IMPL(Input, bool, KeyUp, (uint32_t key_id), (key_id)) { return !m_Frames[0].key_states.at(key_id); }

}  // namespace DE