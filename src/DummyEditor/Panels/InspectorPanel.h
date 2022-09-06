#pragma once

#include "DummyEngineInclude.h"

namespace DE
{
    class InspectorPanel : public ImGuiPanel
    {
    public:
        InspectorPanel() : ImGuiPanel("Inspector") {}
        virtual void View() override;
        
        void SetActiveEntity(Entity entity);

    private:
        Entity m_Entity;
    };
}  // namespace DE