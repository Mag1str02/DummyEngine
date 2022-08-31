#pragma once

#include "Addition/Base.h"

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

        const std::string& GetName() const
        {
            return m_Name;
        }

    protected:
        std::string m_Name;
    };

}  // namespace DE