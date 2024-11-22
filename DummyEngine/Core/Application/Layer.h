#pragma once

#include "DummyEngine/Core/Application/Event.h"

namespace DummyEngine {

    class Layer {
    public:
        explicit Layer(const std::string& name = "Layer") : m_Name(name) {}
        virtual ~Layer() = default;

        virtual void OnAttach() {}
        virtual void OnDetach() {}
        virtual void OnUpdate(float) {}
        virtual void OnImGuiRender() {}
        virtual void OnEvent(Event&) {}

        const std::string& GetName() const { return m_Name; }

        void BroadcastEvent(Event& event) { event_callback_(event); }

    protected:
        std::string m_Name;

    private:
        friend class Application;

        EventCallback<Event> event_callback_;
    };

}  // namespace DummyEngine