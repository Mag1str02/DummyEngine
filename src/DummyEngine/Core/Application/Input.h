#pragma once

#include <GLFW/glfw3.h>

#include "Addition/Base.h"
#include "Core/Application/Event.h"

namespace DE
{

    struct InputFrame
    {
        bool mouse_locked = false;
        double x_pos;
        double y_pos;
        std::array<bool, GLFW_KEY_LAST> key_states;

        InputFrame();
    };

    class Input
    {
    private:
        InputFrame m_CurrentFrame;
        EventDispatcher m_EventDispatcher;
        std::deque<InputFrame> m_Frames;

        void IOnEvent(Event& e);
        void INewFrame();

        double ICursorXOffset() const;
        double ICursorYOffset() const;
        bool IMouseLocked() const;

        bool IKeyReleased(uint32_t key_id) const;
        bool IKeyPressed(uint32_t key_id) const;
        bool IKeyDown(uint32_t key_id) const;
        bool IKeyUp(uint32_t key_id) const;

        Input();
        static Input& Get();

    public:
        static void OnEvent(Event& e);
        static void NewFrame();

        static double CursorXOffset();
        static double CursorYOffset();
        static bool MouseLocked();

        static bool KeyReleased(uint32_t key_id);
        static bool KeyPressed(uint32_t key_id);
        static bool KeyDown(uint32_t key_id);
        static bool KeyUp(uint32_t key_id);
    };
}  // namespace DE