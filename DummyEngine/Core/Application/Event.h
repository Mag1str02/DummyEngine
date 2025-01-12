#pragma once

#include "DummyEngine/Utils/Types/Types.h"

namespace DummyEngine {
    template <typename Event> using EventCallback = std::function<void(Event&)>;

    enum class EventType {
        None = 0,

        //*Responding

        WindowResize,
        WindowClose,

        KeyPressed,
        KeyReleased,

        MouseButtonPressed,
        MouseButtonReleased,
        MouseScrolled,
        MouseMoved,

        //*Triggering

        SetWindowModeWindowed,
        SetWindowModeFullscreen,

        SetMouseLock,
        SetMouseUnlock,
        SetMouseLockToggle,

        Count
    };

    class Event {
    public:
        virtual ~Event()                  = default;
        virtual EventType GetType() const = 0;
    };

    class EventDispatcher {
    public:
        template <typename ListeningEvent> void AddEventListener(EventCallback<ListeningEvent> callback) {
            auto base_callback = [func = std::move(callback)](Event& e) { func(static_cast<ListeningEvent&>(e)); };
            event_callbacks_[(size_t)ListeningEvent::Type()].push_back(base_callback);
        }

        void Dispatch(Event& event) {
            for (auto& callback : event_callbacks_[(size_t)(event.GetType())]) {
                callback(event);
            }
        }

    private:
        std::array<std::vector<EventCallback<Event>>, static_cast<size_t>(EventType::Count)> event_callbacks_;
    };

#define EVENT_TYPE(type)                         \
    virtual EventType GetType() const override { \
        return EventType::type;                  \
    }                                            \
    static EventType Type() {                    \
        return EventType::type;                  \
    }

    //*Responding

    class WindowResizeEvent : public Event {
    public:
        WindowResizeEvent(U32 width, U32 height) : width_(width), height_(height) {}

        U32 GetWidth() const { return width_; }
        U32 GetHeight() const { return height_; }

        EVENT_TYPE(WindowResize);

    private:
        U32 width_;
        U32 height_;
    };
    class WindowCloseEvent : public Event {
    public:
        WindowCloseEvent() {}

        EVENT_TYPE(WindowClose);
    };

    class KeyPressedEvent : public Event {
    public:
        explicit KeyPressedEvent(U32 key) : key_code_(key) {}

        U32 GetKey() const { return key_code_; }

        EVENT_TYPE(KeyPressed);

    private:
        U32 key_code_;
    };
    class KeyReleasedEvent : public Event {
    public:
        explicit KeyReleasedEvent(U32 key) : key_code_(key) {}

        U32 GetKey() const { return key_code_; }

        EVENT_TYPE(KeyReleased);

    private:
        U32 key_code_;
    };

    class MouseButtonPressedEvent : public Event {
    public:
        explicit MouseButtonPressedEvent(U32 key) : key_code_(key) {}

        U32 GetKey() const { return key_code_; }

        EVENT_TYPE(MouseButtonPressed);

    private:
        U32 key_code_;
    };
    class MouseButtonReleasedEvent : public Event {
    public:
        explicit MouseButtonReleasedEvent(U32 key) : key_code_(key) {}

        U32 GetKey() const { return key_code_; }

        EVENT_TYPE(MouseButtonReleased);

    private:
        U32 key_code_;
    };
    class MouseScrolledEvent : public Event {
    public:
        MouseScrolledEvent(float x_offset, float y_offset) : x_pos_(x_offset), y_pos_(y_offset) {}

        float GetXOffset() const { return x_pos_; }
        float GetYOffset() const { return y_pos_; }

        EVENT_TYPE(MouseScrolled);

    private:
        float x_pos_;
        float y_pos_;
    };
    class MouseMovedCallback : public Event {
    public:
        MouseMovedCallback(float x_pos, float y_pos) : x_pos_(x_pos), y_pos_(y_pos) {}

        float GetXPos() const { return x_pos_; }
        float GetYPos() const { return y_pos_; }

        EVENT_TYPE(MouseMoved);

    private:
        float x_pos_;
        float y_pos_;
    };

    //*Triggering

    class SetWindowModeWindowedEvent : public Event {
    public:
        explicit SetWindowModeWindowedEvent(U32 width = 1280, U32 height = 720, U32 x_pos = 100, U32 y_pos = 100) :
            width_(width), height_(height), x_pos_(x_pos), y_pos_(y_pos) {}

        U32 GetWidth() const { return width_; }
        U32 GetHeight() const { return height_; }
        U32 GetXPos() const { return x_pos_; }
        U32 GetYPos() const { return y_pos_; }

        EVENT_TYPE(SetWindowModeWindowed);

    private:
        U32 width_;
        U32 height_;
        U32 x_pos_;
        U32 y_pos_;
    };
    class SetWindowModeFullscreenEvent : public Event {
    public:
        explicit SetWindowModeFullscreenEvent(U32 monitor_id) : monitor_id_(monitor_id) {}

        U32 GetMonitorId() const { return monitor_id_; }

        EVENT_TYPE(SetWindowModeFullscreen);

    private:
        U32 monitor_id_;
    };

    class SetMouseLockEvent : public Event {
    public:
        SetMouseLockEvent() {}

        EVENT_TYPE(SetMouseLock);
    };
    class SetMouseUnlockEvent : public Event {
    public:
        SetMouseUnlockEvent() {}

        EVENT_TYPE(SetMouseUnlock);
    };
    class SetMouseLockToggleEvent : public Event {
    public:
        SetMouseLockToggleEvent() {}

        EVENT_TYPE(SetMouseLockToggle);
    };

}  // namespace DummyEngine