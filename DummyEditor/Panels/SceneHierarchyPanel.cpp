#include "DummyEditor/Panels/SceneHierarchyPanel.h"

namespace DE {
    void SceneHierarchyPanel::SetActiveScene(Ref<Scene> scene) {
        m_Scene = scene;
    }
    void SceneHierarchyPanel::View() {
        DE_PROFILE_SCOPE("SceneHierarchyPanel View");

        if (m_Scene) {
            m_From = nullptr;
            m_To   = nullptr;

            ShowNode(m_Scene->GetHierarchy());

            if (m_From && m_To) {
                if (m_To->IsEntity() && m_From->IsEntity()) {
                    auto parent = m_To->GetParent();
                    auto node   = CreateRef<SceneHierarchyNode>("Folder");

                    auto ref = m_From->Detach();
                    m_To->Detach();
                    node->AttachChild(m_To);
                    node->AttachChild(ref);
                    parent->AttachChild(node);
                }
                if (!m_To->IsEntity() && !m_To->IsAnsestor(m_From)) {
                    auto ref = m_From->Detach();
                    m_To->AttachChild(ref);
                }
            }
        }
    }

    void SceneHierarchyPanel::UnSelect() {
        m_SelectedNode = nullptr;
    }
    Entity SceneHierarchyPanel::GetActiveEntity() {
        if (m_SelectedNode) {
            return m_SelectedNode->GetEntity();
        }
        return Entity();
    }

    void SceneHierarchyPanel::ShowNode(Ref<SceneHierarchyNode> node) {
        bool open;
        if (!node->IsEntity()) {
            open = ImGui::TreeNode(node->GetName().c_str());
            DropTarget(node);
            if (node->GetParent()) {
                DragTarget(node);
            }
            if (open) {
                S32 id = 0;
                for (auto child : *node) {
                    ImGui::PushID(id++);
                    ShowNode(child);
                    ImGui::PopID();
                }

                ImGui::TreePop();
            }
        } else {
            auto entity = node->GetEntity();
            open        = ImGui::Selectable(entity.Get<TagComponent>().Get().c_str(), &node->Selected(), ImGuiSelectableFlags_AllowDoubleClick);
            if (open) {
                if (ImGui::IsMouseDoubleClicked(0)) {
                    m_SelectedNode = node;
                }
            }
            DropTarget(node);
            DragTarget(node);
        }
    }

    void SceneHierarchyPanel::DragTarget(Ref<SceneHierarchyNode> node) {
        if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceNoHoldToOpenOthers)) {
            size_t ptr = (size_t)node.get();
            ImGui::SetDragDropPayload("DND_HIERARCHY_NODE", &ptr, sizeof(size_t), ImGuiCond_Once);
            ImGui::Text("Moving: %s", node->GetName().c_str());
            ImGui::EndDragDropSource();
        }
    }
    void SceneHierarchyPanel::DropTarget(Ref<SceneHierarchyNode> node) {
        if (ImGui::BeginDragDropTarget()) {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_HIERARCHY_NODE")) {
                m_From = (SceneHierarchyNode*)(*(size_t*)payload->Data);
                m_To   = node;
            }
            ImGui::EndDragDropTarget();
        }
    }
}  // namespace DE