#pragma once

#include "DummyEditor/ImGuiUtils/ImGuiPanel.h"

#include "DummyEngine/Core/Scene/Scene.h"

namespace DummyEngine {

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

        WeakRef<Scene>       scene_;
        SceneHierarchy::Node selected_node_;
        SceneHierarchy::Node from_;
        SceneHierarchy::Node to_;
        SceneHierarchy::Node rename_;
        bool                 was_target_ = false;
    };

}  // namespace DummyEngine