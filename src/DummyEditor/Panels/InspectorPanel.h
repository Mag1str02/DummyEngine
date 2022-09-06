#pragma once

#include "DummyEngineInclude.h"

namespace DE
{
    class InspectorPanel
    {
    public:
        InspectorPanel(){}

        void OnImGuiRender();
        void SetCurrentEntity(Entity entity);
    private:
        Entity m_Entity;
    };
}  // namespace DE