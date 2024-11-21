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
        event_dispatcher_.AddEventListener<KeyPressedEvent>([this](KeyPressedEvent& event) {
            DE_ASSERT(current_frame_.KeyStates.size() > event.GetKey(), "Bad key code: {}", event.GetKey());
            current_frame_.KeyStates[event.GetKey()] = true;
        });
        event_dispatcher_.AddEventListener<KeyReleasedEvent>([this](KeyReleasedEvent& event) {
            DE_ASSERT(current_frame_.KeyStates.size() > event.GetKey(), "Bad key code: {}", event.GetKey());
            current_frame_.KeyStates[event.GetKey()] = false;
        });

        event_dispatcher_.AddEventListener<SetMouseLockEvent>([this](SetMouseLockEvent&) { current_frame_.MouseLocked = true; });
        event_dispatcher_.AddEventListener<SetMouseUnlockEvent>([this](SetMouseUnlockEvent&) { current_frame_.MouseLocked = false; });
        event_dispatcher_.AddEventListener<SetMouseLockToggleEvent>(
            [this](SetMouseLockToggleEvent&) { current_frame_.MouseLocked = !current_frame_.MouseLocked; });

        event_dispatcher_.AddEventListener<MouseMovedCallback>([this](MouseMovedCallback& event) {
            current_frame_.PosX = event.GetXPos();
            current_frame_.PosY = event.GetYPos();
        });

        INewFrame();
        return Unit();
    }
    S_TERMINATE() {
        return Unit();
    }

    S_METHOD_IMPL(Unit, OnEvent, (Event & event), (event)) {
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