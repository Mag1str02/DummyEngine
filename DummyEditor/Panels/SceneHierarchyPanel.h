#pragma once

#include "DummyEditor/ImGuiUtils/ImGuiPanel.h"

namespace DE {
    class SceneHierarchyPanel : public ImGuiPanel {
    public:
        SceneHierarchyPanel() : ImGuiPanel(ICON_MD_ACCOUNT_TREE "  Scene Hierarchy") {}
        virtual void View() override;

        void   SetActiveScene(WeakRef<Scene> scene);
        Entity GetActiveEntity();

    private:
        void ShowNode(SceneHierarchy::Node node);
        void DragTarget(SceneHierarchy::Node node);
        void DropTarget(SceneHierarchy::Node node);

        WeakRef<Scene>       m_Scene;
        SceneHierarchy::Node m_SelectedNode;
        SceneHierarchy::Node m_From;
        SceneHierarchy::Node m_To;
    };
}  // namespace DE