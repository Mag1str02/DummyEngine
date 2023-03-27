#pragma once

#include "DummyEngine/Core/Application/Event.h"
#include "DummyEngine/Core/Application/KeyCodes.h"
#include "DummyEngine/Utils/Base.h"

namespace DE {

    struct InputFrame {
        bool              mouse_locked = false;
        double            x_pos;
        double            y_pos;
        std::vector<bool> key_states;

        InputFrame();
    };

    class Input : public Singleton<Input> {
        SINGLETON(Input)
    public:
        S_METHOD_DEF(Unit, OnEvent, (Event & e));
        S_METHOD_DEF(Unit, NewFrame, ());
        S_METHOD_DEF(Unit, SetFrameAmount, (size_t n));

        S_METHOD_DEF(double, CursorXOffset, ());
        S_METHOD_DEF(double, CursorYOffset, ());

        S_METHOD_DEF(bool, MouseLocked, ());
        S_METHOD_DEF(bool, KeyReleased, (Key key));
        S_METHOD_DEF(bool, KeyPressed, (Key key));
        S_METHOD_DEF(bool, KeyDown, (Key key));
        S_METHOD_DEF(bool, KeyUp, (Key key));

    private:
        InputFrame             m_CurrentFrame;
        EventDispatcher        m_EventDispatcher;
        std::deque<InputFrame> m_Frames;
        size_t                 m_MaxFrameAmount = 100;
    };
}  // namespace DE