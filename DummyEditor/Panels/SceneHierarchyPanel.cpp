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
            if (m_From.IsEntity() && m_To.IsEntity()) {
                auto parent = m_To.GetParent();
                auto folder = parent.AddFolder("Folder");
                folder.Attach(m_To);
                folder.Attach(m_From);
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
            ImGui::PushID(node.GetID());
            std::string name = ICON_MD_FOLDER "  " + node.GetName();
            open             = ImGui::TreeNodeEx(name.c_str(), ImGuiTreeNodeFlags_SpanFullWidth);
            DropTarget(node);
            DragTarget(node);
            if (open) {
                for (auto child : node.GetChilds()) {
                    ShowNode(child);
                }
                ImGui::TreePop();
            }
            ImGui::PopID();
        }
        if (node.IsEntity()) {
            ImGui::PushID(node.GetID());
            auto        entity = node.GetEntity();
            std::string name   = "        " ICON_MD_CHECK_BOX_OUTLINE_BLANK "  " + entity.Get<TagComponent>().Get();
            if (ImGui::Selectable(name.c_str(), false, ImGuiSelectableFlags_AllowDoubleClick | ImGuiSelectableFlags_SpanAllColumns) &&
                ImGui::IsMouseDoubleClicked(0)) {
                m_SelectedNode = node;
            }

            DropTarget(node);
            DragTarget(node);
            ImGui::PopID();
        }
    }

    void SceneHierarchyPanel::DragTarget(SceneHierarchy::Node node) {
        if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceNoHoldToOpenOthers)) {
            if (m_To.IsEntity() && node.IsEntity()) {
                ImGui::Text("Create folder");
            }
            if (m_To.IsFolder()) {
                if (node.IsEntity()) {
                    ImGui::Text("Move %s to %s", node.GetEntity().Get<TagComponent>().Get().c_str(), m_To.GetName().c_str());
                }
                if (node.IsFolder()) {
                    ImGui::Text("Move %s to %s", node.GetName().c_str(), m_To.GetName().c_str());
                }
            }
            ImGui::SetDragDropPayload("DND_HIERARCHY_NODE", &node, sizeof(node), ImGuiCond_Once);
            ImGui::EndDragDropSource();
        }
    }
    void SceneHierarchyPanel::DropTarget(SceneHierarchy::Node node) {
        if (ImGui::BeginDragDropTarget()) {
            m_To = node;
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_HIERARCHY_NODE")) {
                m_From = *(SceneHierarchy::Node*)payload->Data;
            }
            ImGui::EndDragDropTarget();
        }
    }
}  // namespace DE