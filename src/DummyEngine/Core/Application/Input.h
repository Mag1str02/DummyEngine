#pragma once

#include "Addition/Base.h"
#include "Core/Application/Event.h"

namespace DE
{

    struct InputFrame
    {
        double x_pos;
        double y_pos;
        std::map<int16_t, bool> key_states;

        InputFrame();
    };

    class Input
    {
    private:
        InputFrame m_CurrentFrame;
        std::deque<InputFrame> m_Frames;

        void IOnEvent(Event& e);
        void INewFrame();

        double ICursorXOffset() const;
        double ICursorYOffset() const;
        bool IKeyReleased(int16_t key_id) const;
        bool IKeyPressed(int16_t key_id) const;
        bool IKeyDown(int16_t key_id) const;
        bool IKeyUp(int16_t key_id) const;

        Input(){}
        static Input& Get();

    public:
        static void OnEvent(Event& e);
        static void NewFrame();

        static double CursorXOffset();
        static double CursorYOffset();
        static bool KeyReleased(int16_t key_id);
        static bool KeyPressed(int16_t key_id);
        static bool KeyDown(int16_t key_id);
        static bool KeyUp(int16_t key_id);
    };
}  // namespace DE