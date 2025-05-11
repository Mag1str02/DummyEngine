#pragma once

#include "DummyEngine/Core/Application/Event.h"
#include "DummyEngine/Core/Application/KeyCodes.h"
#include "DummyEngine/Utils/Helpers/Singleton.h"

#include <deque>

namespace DummyEngine {

    struct InputFrame {
        bool              MouseLocked = false;
        double            PosX;
        double            PosY;
        std::vector<bool> KeyStates;

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
        InputFrame             current_frame_;
        EventDispatcher        event_dispatcher_;
        std::deque<InputFrame> frames_;
        size_t                 max_frame_amount_;
    };

}  // namespace DummyEngine