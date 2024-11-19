#pragma once

#include "DummyEditor/ImGuiUtils/ImGuiPanel.h"

namespace DE {
    class InspectorPanel : public ImGuiPanel {
    public:
        InspectorPanel() : ImGuiPanel("Inspector") {}
        virtual void OnImGui() override;

        void AddComponent();
        void SetActiveEntity(Entity entity);
        void SetScene(WeakRef<Scene> scene);

    private:
        template <typename Component> void DrawComponentWidget();

        Entity         m_Entity;
        WeakRef<Scene> m_Scene;
    };
}  // namespace DE