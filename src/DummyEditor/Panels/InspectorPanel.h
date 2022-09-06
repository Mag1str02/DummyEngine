#pragma once

#include "DummyEngineInclude.h"

namespace DE
{
    class InspectorPanel
    {
    public:
        InspectorPanel() {}

        void OnImGuiRender(bool& enabled);
        void SetCurrentEntity(Entity entity);

    private:
        Entity m_Entity;
    };
}  // namespace DE