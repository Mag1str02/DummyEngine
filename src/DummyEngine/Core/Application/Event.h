#pragma once

#include "Addition/Base.h"

namespace DE
{
    template <typename Event> using EventCallback = std::function<void(Event&)>;

    enum class EventType
    {
        None = 0,

        WindowResize,
        WindowClose,

        KeyPressed,
        KeyReleased,

        MouseButtonPressed,
        MouseButtonReleased,
        MouseScrolled,
        MouseMoved,

        Count
    };

    class Event
    {
    public:
        virtual ~Event() = default;
        virtual EventType GetType() const = 0;
    };

    class EventDispatcher
    {
    public:
        template <typename ListeningEvent> void AddEventListener(EventCallback<ListeningEvent> callback)
        {
            auto base_callback = [func = std::move(callback)](Event& e) { func(static_cast<ListeningEvent&>(e)); };
            m_EventCallbacks[(size_t)ListeningEvent::Type()] = std::move(base_callback);
        }

        void Dispatch(Event& event)
        {
            auto& callback = m_EventCallbacks[(size_t)(event.GetType())];
            if (callback)
            {
                callback(event);
            }
        }

    private:
        std::array<EventCallback<Event>, static_cast<size_t>(EventType::Count)> m_EventCallbacks;
    };

#define EVENT_TYPE(type)                       \
    virtual EventType GetType() const override \
    {                                          \
        return EventType::type;                \
    }                                          \
    static EventType Type()                    \
    {                                          \
        return EventType::type;                \
    }

    class WindowResizeEvent : public Event
    {
    public:
        WindowResizeEvent(uint32_t width, uint32_t height) : m_Width(width), m_Height(height) {}

        uint32_t GetWidth() const
        {
            return m_Width;
        }
        uint32_t GetHeight() const
        {
            return m_Height;
        }

        EVENT_TYPE(WindowResize);

    private:
        uint32_t m_Width;
        uint32_t m_Height;
    };
    class WindowCloseEvent : public Event
    {
    public:
        WindowCloseEvent() {}

        EVENT_TYPE(WindowClose);
    };

    class KeyPressedEvent : public Event
    {
    public:
        KeyPressedEvent(uint32_t key) : m_KeyKode(key) {}

        uint32_t GetKey() const
        {
            return m_KeyKode;
        }

        EVENT_TYPE(KeyPressed);

    private:
        uint32_t m_KeyKode;
    };
    class KeyReleasedEvent : public Event
    {
    public:
        KeyReleasedEvent(uint32_t key) : m_KeyKode(key) {}

        uint32_t GetKey() const
        {
            return m_KeyKode;
        }

        EVENT_TYPE(KeyPressed);

    private:
        uint32_t m_KeyKode;
    };

    class MouseButtonPressedEvent : public Event
    {
    public:
        MouseButtonPressedEvent(uint32_t key) : m_KeyKode(key) {}

        uint32_t GetKey() const
        {
            return m_KeyKode;
        }

        EVENT_TYPE(MouseButtonPressed);

    private:
        uint32_t m_KeyKode;
    };
    class MouseButtonReleasedEvent : public Event
    {
    public:
        MouseButtonReleasedEvent(uint32_t key) : m_KeyKode(key) {}

        uint32_t GetKey() const
        {
            return m_KeyKode;
        }

        EVENT_TYPE(MouseButtonReleased);

    private:
        uint32_t m_KeyKode;
    };
    class MouseScrolledEvent : public Event
    {
    public:
        MouseScrolledEvent(float x_offset, float y_offset) : m_XOffset(x_offset), m_YOffset(y_offset) {}

        float GetXOffset() const
        {
            return m_XOffset;
        }
        float GetYOffset() const
        {
            return m_YOffset;
        }

        EVENT_TYPE(MouseScrolled);

    private:
        float m_XOffset;
        float m_YOffset;
    };
    class MouseMovedEvent : public Event
    {
    public:
        MouseMovedEvent(float x_offset, float y_offset) : m_XOffset(x_offset), m_YOffset(y_offset) {}

        float GetXOffset() const
        {
            return m_XOffset;
        }
        float GetYOffset() const
        {
            return m_YOffset;
        }

        EVENT_TYPE(MouseMoved);

    private:
        float m_XOffset;
        float m_YOffset;
    };

}  // namespace DE