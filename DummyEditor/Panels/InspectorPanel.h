#pragma once

#include "DummyEditor/ImGuiUtils/ImGuiPanel.h"

#include "DummyEngine/Core/ECS/ECS.h"

namespace DummyEngine {
    class Scene;

    class InspectorPanel : public ImGuiPanel {
    public:
        InspectorPanel() : ImGuiPanel("Inspector") {}
        virtual void OnImGui() override;

        void AddComponent();
        void SetActiveEntity(Entity entity);
        void SetScene(WeakRef<Scene> scene);

    private:
        template <typename Component> void DrawComponentWidget();

        Entity         entity_;
        WeakRef<Scene> scene_;
    };
}  // namespace DummyEngine