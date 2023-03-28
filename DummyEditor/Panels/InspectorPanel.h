#pragma once

#include "DummyEditor/DummyEngineInclude.h"

namespace DE {
    class InspectorPanel : public ImGuiPanel {
    public:
        InspectorPanel() : ImGuiPanel("Inspector") {}
        virtual void View() override;

        void SetActiveEntity(Entity entity);
        void SetScene(WeakRef<Scene> scene);

    private:
        Entity         m_Entity;
        WeakRef<Scene> m_Scene;
    };
}  // namespace DE