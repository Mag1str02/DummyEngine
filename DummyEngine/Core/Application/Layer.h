#pragma once

#include "DummyEngine/Core/Application/Event.h"

namespace DummyEngine {

    class Layer {
    public:
        explicit Layer(const std::string& name = "Layer");
        virtual ~Layer() = default;

        virtual void OnAttach() {}
        virtual void OnDetach() {}
        virtual void OnUpdate(float) {}
        virtual void OnImGuiRender() {}
        virtual void OnEvent(const Event&) {}

        const std::string& GetName() const;

        void BroadcastEvent(const Event& event);

    protected:
        std::string m_Name;
    };

}  // namespace DummyEngine