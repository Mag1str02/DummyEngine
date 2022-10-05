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
        void DragTarget(Ref<SceneHierarchyNode> node);
        void DropTarget(Ref<SceneHierarchyNode> node);

        Ref<Scene>              m_Scene;
        Ref<SceneHierarchyNode> m_SelectedNode;
        SceneHierarchyNode*     m_From;
        Ref<SceneHierarchyNode> m_To;
    };
}  // namespace DE