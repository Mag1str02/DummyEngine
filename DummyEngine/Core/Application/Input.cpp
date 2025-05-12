#include "Input.h"

#include "DummyEngine/Core/Application/Config.h"
#include "DummyEngine/Utils/Debug/Profiler.h"

namespace DummyEngine {
    InputFrame::InputFrame() {
        KeyStates.resize((size_t)Key::Last);
    }

    SINGLETON_BASE(Input);
    S_INITIALIZE() {
        max_frame_amount_ = Config::Get().MaxInputFrameAmount;

        event_dispatcher_.AddEventListener<KeyEvent>([this](const KeyEvent& event) {
            DE_ASSERT(current_frame_.KeyStates.size() > (U32)event.Key, "Bad key code: {}", event.Key);
            current_frame_.KeyStates[event.Key] = event.Action;
        });

        event_dispatcher_.AddEventListener<SetMouseLockEvent>([this](const SetMouseLockEvent& event) {
            switch (event.Action) {
                case SetMouseLockEvent::Lock: current_frame_.MouseLocked = true; return;
                case SetMouseLockEvent::UnLock: current_frame_.MouseLocked = false; return;
                case SetMouseLockEvent::Switch: current_frame_.MouseLocked = !current_frame_.MouseLocked; return;
                default: DE_ASSERT(false, "Invalid value of action: {}", (U32)event.Action);
            }
        });

        event_dispatcher_.AddEventListener<MousePositionEvent>([this](const MousePositionEvent& event) {
            current_frame_.PosX = event.X;
            current_frame_.PosY = event.Y;
        });

        INewFrame();
        return Unit();
    }
    S_TERMINATE() {
        return Unit();
    }

    S_METHOD_IMPL(Unit, OnEvent, (const Event& event), (event)) {
        event_dispatcher_.Dispatch(event);
        return Unit();
    }
    S_METHOD_IMPL(Unit, NewFrame, (), ()) {
        DE_PROFILE_SCOPE("Imput Frame Begin");

        frames_.push_front(current_frame_);
        if (frames_.size() > max_frame_amount_) {
            frames_.pop_back();
        }
        return Unit();
    }
    S_METHOD_IMPL(Unit, SetFrameAmount, (size_t n), (n)) {
        max_frame_amount_ = n;
        return Unit();
    }

    S_METHOD_IMPL(double, CursorXOffset, (), ()) {
        return frames_[0].PosX - frames_[1].PosX;
    }
    S_METHOD_IMPL(double, CursorYOffset, (), ()) {
        return frames_[0].PosY - frames_[1].PosY;
    }

    S_METHOD_IMPL(bool, MouseLocked, (), ()) {
        return frames_[0].MouseLocked;
    }
    S_METHOD_IMPL(bool, KeyReleased, (Key key), (key)) {
        return !frames_[0].KeyStates.at((size_t)key) && frames_[1].KeyStates.at((size_t)key);
    }
    S_METHOD_IMPL(bool, KeyPressed, (Key key), (key)) {
        return frames_[0].KeyStates.at((size_t)key) && !frames_[1].KeyStates.at((size_t)key);
    }
    S_METHOD_IMPL(bool, KeyDown, (Key key), (key)) {
        return frames_[0].KeyStates.at((size_t)key);
    }
    S_METHOD_IMPL(bool, KeyUp, (Key key), (key)) {
        return !frames_[0].KeyStates.at((size_t)key);
    }

}  // namespace DummyEngine