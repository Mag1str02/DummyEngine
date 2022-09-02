#pragma once

#include "Addition/Base.h"
#include "Core/Application/Event.h"

namespace DE
{
    class Layer
    {
    public:
        Layer(const std::string& name = "Layer") : m_Name(name) {}
        virtual ~Layer() = default;

        virtual void OnAttach() {}
        virtual void OnDetach() {}
        virtual void OnUpdate(float dt) {}
        virtual void OnImGuiRender() {}
        virtual void OnEvent(Event& event) {}

        const std::string& GetName() const
        {
            return m_Name;
        }

        void BroadcastEvent(Event& event)
        {
            m_EventCallback(event);
        }

    protected:
        std::string m_Name;

    private:
        friend class Application;

        EventCallback<Event> m_EventCallback;
    };

}  // namespace DE