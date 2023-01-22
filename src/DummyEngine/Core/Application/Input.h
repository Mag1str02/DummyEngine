#pragma once

#include "DummyEngine/Utils/Base.h"
#include "DummyEngine/Core/Application/Event.h"
#include "DummyEngine/Core/Application/KeyCodes.h"

namespace DE
{

    struct InputFrame
    {
        bool              mouse_locked = false;
        double            x_pos;
        double            y_pos;
        std::vector<bool> key_states;

        InputFrame();
    };

    class Input : public Singleton<Input>
    {
        SINGLETON(Input)
    public:
        S_METHOD_DEF(Input, Unit, Initialize, ());
        S_METHOD_DEF(Input, Unit, Terminate, ());

        S_METHOD_DEF(Input, Unit, OnEvent, (Event & e));
        S_METHOD_DEF(Input, Unit, NewFrame, ());

        S_METHOD_DEF(Input, double, CursorXOffset, ());
        S_METHOD_DEF(Input, double, CursorYOffset, ());

        S_METHOD_DEF(Input, bool, MouseLocked, ());
        S_METHOD_DEF(Input, bool, KeyReleased, (uint32_t key_id));
        S_METHOD_DEF(Input, bool, KeyPressed, (uint32_t key_id));
        S_METHOD_DEF(Input, bool, KeyDown, (uint32_t key_id));
        S_METHOD_DEF(Input, bool, KeyUp, (uint32_t key_id));

    private:
        Input()  = default;
        ~Input() = default;

        InputFrame             m_CurrentFrame;
        EventDispatcher        m_EventDispatcher;
        std::deque<InputFrame> m_Frames;
    };
}  // namespace DE