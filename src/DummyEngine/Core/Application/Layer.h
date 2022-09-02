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
        virtual void OnEvent(Event& e) {}

        const std::string& GetName() const
        {
            return m_Name;
        }

    protected:
        std::string m_Name;
    };

}  // namespace DE