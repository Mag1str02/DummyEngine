#include "DummyEditor/Panels/SceneHierarchyPanel.h"

namespace DE {
    void SceneHierarchyPanel::SetActiveScene(WeakRef<Scene> scene) {
        m_Scene        = scene;
        m_SelectedNode = SceneHierarchy::Node();
    }
    void SceneHierarchyPanel::View() {
        DE_PROFILE_SCOPE("SceneHierarchyPanel View");
        auto scene = m_Scene.lock();
        if (scene) {
            m_From = SceneHierarchy::Node();
            m_To   = SceneHierarchy::Node();

            ShowNode(scene->GetHierarchy().GetRoot());

            if (m_From.Valid() && m_To.IsFolder()) {
                m_To.Attach(m_From);
            }
        }
    }

    Entity SceneHierarchyPanel::GetActiveEntity() {
        auto scene = m_Scene.lock();
        if (scene && m_SelectedNode.IsEntity()) {
            return m_SelectedNode.GetEntity();
        }
        return Entity();
    }

    void SceneHierarchyPanel::ShowNode(SceneHierarchy::Node node) {
        bool open;
        if (node.IsFolder()) {
            open = ImGui::TreeNode(node.GetName().c_str());
            DropTarget(node);
            DragTarget(node);
            if (open) {
                for (auto child : node.GetChilds()) {
                    ImGui::PushID(node.GetID());
                    ShowNode(child);
                    ImGui::PopID();
                }
                ImGui::TreePop();
            }
        }
        if (node.IsEntity()) {
            auto entity = node.GetEntity();
            open        = ImGui::Selectable(entity.Get<TagComponent>().Get().c_str(), false, ImGuiSelectableFlags_AllowDoubleClick);
            if (open) {
                if (ImGui::IsMouseDoubleClicked(0)) {
                    m_SelectedNode = node;
                }
            }
            DropTarget(node);
            DragTarget(node);
        }
    }

    void SceneHierarchyPanel::DragTarget(SceneHierarchy::Node node) {
        if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceNoHoldToOpenOthers)) {
            ImGui::SetDragDropPayload("DND_HIERARCHY_NODE", &node, sizeof(node), ImGuiCond_Once);
            ImGui::EndDragDropSource();
        }
    }
    void SceneHierarchyPanel::DropTarget(SceneHierarchy::Node node) {
        if (ImGui::BeginDragDropTarget()) {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_HIERARCHY_NODE")) {
                m_From = *(SceneHierarchy::Node*)payload->Data;
                m_To   = node;
            }
            ImGui::EndDragDropTarget();
        }
    }
}  // namespace DE