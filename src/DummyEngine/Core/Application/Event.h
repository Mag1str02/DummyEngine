#pragma once

#include "Addition/Base.h"

namespace DE
{
    template <typename Event> using EventCallBack = std::function<void(Event&)>;

    enum class EventType
    {
        None = 0,

        WindowResize,
        WindowClose,

        KeyPressed,
        KeyReleased,

        MouseButtonPressed,
        MouseButtonReleased,
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
        template <typename ListeningEvent> void AddEventListener(EventCallBack<ListeningEvent> callback)
        {
            auto base_callback = [func = std::move(callback)](Event& e) { func(static_cast<ListeningEvent&>(e)); };
            m_EventCallbacks[static_cast<size_t>(ListeningEvent::Type)] = std::move(base_callback);
        }

        void Dispatch(Event& event)
        {
            auto& callback = m_EventCallbacks[static_cast<size_t>(event.GetType())];
            if (callback)
            {
                callback(event);
            }
        }

    private:
        std::array<EventCallBack<Event>, static_cast<size_t>(EventType::Count)> m_EventCallbacks;
    };

}  // namespace DE