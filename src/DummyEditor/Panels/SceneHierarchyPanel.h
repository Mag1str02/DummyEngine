#pragma once

#include "DummyEngineInclude.h"

namespace DE
{
    class SceneHierarchyPanel : public ImGuiPanel
    {
    public:
        SceneHierarchyPanel() : ImGuiPanel("Scene Hierarchy") {}
        virtual void View() override;

        void   SetActiveScene(Ref<Scene> scene);
        void   UnSelect();
        Entity GetActiveEntity();

    private:
        void ShowNode(Ref<SceneHierarchyNode> node);

        Ref<Scene>              m_Scene;
        Ref<SceneHierarchyNode> m_SelectedNode;
    };
}  // namespace DE