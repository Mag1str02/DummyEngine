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
        WindowResizeEvent(uint32_t width, uint32_t height) : m_Width(width), m_Height(height) {}

        uint32_t GetWidth() const { return m_Width; }
        uint32_t GetHeight() const { return m_Height; }

        EVENT_TYPE(WindowResize);

    private:
        uint32_t m_Width;
        uint32_t m_Height;
    };
    class WindowCloseEvent : public Event {
    public:
        WindowCloseEvent() {}

        EVENT_TYPE(WindowClose);
    };

    class KeyPressedEvent : public Event {
    public:
        KeyPressedEvent(uint32_t key) : m_KeyKode(key) {}

        uint32_t GetKey() const { return m_KeyKode; }

        EVENT_TYPE(KeyPressed);

    private:
        uint32_t m_KeyKode;
    };
    class KeyReleasedEvent : public Event {
    public:
        KeyReleasedEvent(uint32_t key) : m_KeyKode(key) {}

        uint32_t GetKey() const { return m_KeyKode; }

        EVENT_TYPE(KeyReleased);

    private:
        uint32_t m_KeyKode;
    };

    class MouseButtonPressedEvent : public Event {
    public:
        MouseButtonPressedEvent(uint32_t key) : m_KeyKode(key) {}

        uint32_t GetKey() const { return m_KeyKode; }

        EVENT_TYPE(MouseButtonPressed);

    private:
        uint32_t m_KeyKode;
    };
    class MouseButtonReleasedEvent : public Event {
    public:
        MouseButtonReleasedEvent(uint32_t key) : m_KeyKode(key) {}

        uint32_t GetKey() const { return m_KeyKode; }

        EVENT_TYPE(MouseButtonReleased);

    private:
        uint32_t m_KeyKode;
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
        SetWindowModeWindowedEvent(uint32_t width = 1280, uint32_t height = 720, uint32_t x_pos = 100, uint32_t y_pos = 100) :
            m_Width(width), m_Height(height), m_XPos(x_pos), m_YPos(y_pos) {}

        uint32_t GetWidth() const { return m_Width; }
        uint32_t GetHeight() const { return m_Height; }
        uint32_t GetXPos() const { return m_XPos; }
        uint32_t GetYPos() const { return m_YPos; }

        EVENT_TYPE(SetWindowModeWindowed);

    private:
        uint32_t m_Width;
        uint32_t m_Height;
        uint32_t m_XPos;
        uint32_t m_YPos;
    };
    class SetWindowModeFullscreenEvent : public Event {
    public:
        SetWindowModeFullscreenEvent(uint32_t monitor_id) : m_MonitorId(monitor_id) {}

        uint32_t GetMonitorId() const { return m_MonitorId; }

        EVENT_TYPE(SetWindowModeFullscreen);

    private:
        uint32_t m_MonitorId;
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