#pragma once

#include "DummyEngine/Utils/Base.h"

namespace DE {
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
            m_EventCallbacks[(size_t)ListeningEvent::Type()].push_back(base_callback);
        }

        void Dispatch(Event& event) {
            for (auto& callback : m_EventCallbacks[(size_t)(event.GetType())]) {
                callback(event);
            }
        }

    private:
        std::array<std::vector<EventCallback<Event>>, static_cast<size_t>(EventType::Count)> m_EventCallbacks;
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
        WindowResizeEvent(U32 width, U32 height) : m_Width(width), m_Height(height) {}

        U32 GetWidth() const { return m_Width; }
        U32 GetHeight() const { return m_Height; }

        EVENT_TYPE(WindowResize);

    private:
        U32 m_Width;
        U32 m_Height;
    };
    class WindowCloseEvent : public Event {
    public:
        WindowCloseEvent() {}

        EVENT_TYPE(WindowClose);
    };

    class KeyPressedEvent : public Event {
    public:
        KeyPressedEvent(U32 key) : m_KeyKode(key) {}

        U32 GetKey() const { return m_KeyKode; }

        EVENT_TYPE(KeyPressed);

    private:
        U32 m_KeyKode;
    };
    class KeyReleasedEvent : public Event {
    public:
        KeyReleasedEvent(U32 key) : m_KeyKode(key) {}

        U32 GetKey() const { return m_KeyKode; }

        EVENT_TYPE(KeyReleased);

    private:
        U32 m_KeyKode;
    };

    class MouseButtonPressedEvent : public Event {
    public:
        MouseButtonPressedEvent(U32 key) : m_KeyKode(key) {}

        U32 GetKey() const { return m_KeyKode; }

        EVENT_TYPE(MouseButtonPressed);

    private:
        U32 m_KeyKode;
    };
    class MouseButtonReleasedEvent : public Event {
    public:
        MouseButtonReleasedEvent(U32 key) : m_KeyKode(key) {}

        U32 GetKey() const { return m_KeyKode; }

        EVENT_TYPE(MouseButtonReleased);

    private:
        U32 m_KeyKode;
    };
    class MouseScrolledEvent : public Event {
    public:
        MouseScrolledEvent(float x_offset, float y_offset) : m_XPos(x_offset), m_YPos(y_offset) {}

        float GetXOffset() const { return m_XPos; }
        float GetYOffset() const { return m_YPos; }

        EVENT_TYPE(MouseScrolled);

    private:
        float m_XPos;
        float m_YPos;
    };
    class MouseMovedCallback : public Event {
    public:
        MouseMovedCallback(float x_pos, float y_pos) : m_XPos(x_pos), m_YPos(y_pos) {}

        float GetXPos() const { return m_XPos; }
        float GetYPos() const { return m_YPos; }

        EVENT_TYPE(MouseMoved);

    private:
        float m_XPos;
        float m_YPos;
    };

    //*Triggering

    class SetWindowModeWindowedEvent : public Event {
    public:
        SetWindowModeWindowedEvent(U32 width = 1280, U32 height = 720, U32 x_pos = 100, U32 y_pos = 100) :
            m_Width(width), m_Height(height), m_XPos(x_pos), m_YPos(y_pos) {}

        U32 GetWidth() const { return m_Width; }
        U32 GetHeight() const { return m_Height; }
        U32 GetXPos() const { return m_XPos; }
        U32 GetYPos() const { return m_YPos; }

        EVENT_TYPE(SetWindowModeWindowed);

    private:
        U32 m_Width;
        U32 m_Height;
        U32 m_XPos;
        U32 m_YPos;
    };
    class SetWindowModeFullscreenEvent : public Event {
    public:
        SetWindowModeFullscreenEvent(U32 monitor_id) : m_MonitorId(monitor_id) {}

        U32 GetMonitorId() const { return m_MonitorId; }

        EVENT_TYPE(SetWindowModeFullscreen);

    private:
        U32 m_MonitorId;
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

}  // namespace DE