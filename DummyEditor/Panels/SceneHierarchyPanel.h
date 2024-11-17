#pragma once

#include "DummyEditor/ImGuiUtils/ImGuiPanel.h"

namespace DE {
    class SceneHierarchyPanel : public ImGuiPanel {
    public:
        SceneHierarchyPanel() : ImGuiPanel("Scene Hierarchy") {}
        virtual void OnImGui() override;

        void   SetActiveScene(WeakRef<Scene> scene);
        Entity GetActiveEntity();

    private:
        void        ShowNodeContextMenu(SceneHierarchy::Node node);
        void        ShowNode(SceneHierarchy::Node node);
        void        DragTarget(SceneHierarchy::Node node);
        void        DropTarget(SceneHierarchy::Node node);
        bool        PossibleDND(SceneHierarchy::Node from, SceneHierarchy::Node to);
        std::string GetDNDText(SceneHierarchy::Node from, SceneHierarchy::Node to);

        WeakRef<Scene>       m_Scene;
        SceneHierarchy::Node m_SelectedNode;
        SceneHierarchy::Node m_From;
        SceneHierarchy::Node m_To;
        SceneHierarchy::Node m_Rename;
        bool                 m_WasTarget = false;
    };
}  // namespace DE