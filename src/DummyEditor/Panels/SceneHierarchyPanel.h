#pragma once

#include "DummyEngineInclude.h"

namespace DE
{
    class SceneHierarchyPanel : public ImGuiPanel
    {
    public:
        SceneHierarchyPanel() : ImGuiPanel("Scene Hierarchy") {}
        virtual void View() override;

        void SetActiveScene(Ref<Scene> scene);
        Entity GetActiveEntity();

    private:
        Ref<Scene> m_Scene;
        Entity m_SelectedEntity;
    };
}  // namespace DE