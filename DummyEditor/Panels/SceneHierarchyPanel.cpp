#include "DummyEditor/Panels/SceneHierarchyPanel.h"

namespace DE {
    void SceneHierarchyPanel::SetActiveScene(WeakRef<Scene> scene) {
        m_Scene        = scene;
        m_SelectedNode = SceneHierarchy::Node();
    }
    void SceneHierarchyPanel::OnImGui() {
        DE_PROFILE_SCOPE("SceneHierarchyPanel View");
        if (m_Controller) {
            if (ImGui::Begin(ICON_MD_ACCOUNT_TREE "  Scene Hierarchy")) {
                auto scene = m_Scene.lock();
                if (scene) {
                    if (!m_WasTarget) {
                        m_To = SceneHierarchy::Node();
                    }
                    m_WasTarget = false;
                    ShowNode(scene->GetHierarchyRoot());
                }
            }
            ImGui::End();
        }
    }

    Entity SceneHierarchyPanel::GetActiveEntity() {
        auto scene = m_Scene.lock();
        if (scene && m_SelectedNode.IsEntity()) {
            return m_SelectedNode.GetEntity();
        }
        return Entity();
    }

    void SceneHierarchyPanel::ShowNodeContextMenu(SceneHierarchy::Node node) {
        if (node.IsFolder()) {
            if (ImGui::MenuItem("Create Entity")) {
                auto entity = m_Scene.lock()->CreateEntity("Entity", false);
                node.AddEntity(entity);
                ImGui::CloseCurrentPopup();
            }
            if (ImGui::MenuItem("Create Folder")) {
                node.AddFolder("Folder");
                ImGui::CloseCurrentPopup();
            }
            if (node.GetID() && ImGui::MenuItem("Rename")) {
                m_Rename = node;
                ImGui::CloseCurrentPopup();
            }
            if (node.GetID() && ImGui::MenuItem("Delete")) {
                auto parent = node.GetParent();
                for (auto c : node.GetChilds()) {
                    parent.Attach(c);
                }
                node.Delete();
                ImGui::CloseCurrentPopup();
            }
        }
        if (node.IsEntity()) {
            if (ImGui::MenuItem("Clone")) {
                auto parent = node.GetParent();
                auto entity = m_Scene.lock()->CloneEntity(node.GetEntity());
                parent.AddEntity(entity);
                ImGui::CloseCurrentPopup();
            }
            if (ImGui::MenuItem("Delete")) {
                node.GetEntity().Destroy();
                node.Delete();
                ImGui::CloseCurrentPopup();
            }
        }
    }
    void SceneHierarchyPanel::ShowNode(SceneHierarchy::Node node) {
        bool open;
        if (node.IsFolder()) {
            ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_None;
            if (node != m_Rename) {
                flags |= ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_SpanAvailWidth;
            }
            ImGui::PushID(node.GetID());
            open = ImGui::TreeNodeEx("Node", flags, ICON_MD_FOLDER);
            if (ImGui::BeginPopupContextItem("Node")) {
                ShowNodeContextMenu(node);
                ImGui::EndPopup();
            }
            if (node.Valid()) {
                DropTarget(node);
                DragTarget(node);
                ImGui::SameLine();
                if (m_Rename == node) {
                    if (!ImGui::IsWindowFocused()) {
                        m_Rename = SceneHierarchy::Node();
                    } else {
                        ImGui::SetKeyboardFocusHere();
                        if (ImGui::InputText(
                                "###FolderRename", &m_Rename.GetName(), ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue)) {
                            m_Rename = SceneHierarchy::Node();
                        }
                    }
                } else {
                    ImGui::Text("%s", node.GetName().c_str());
                }
            }
            if (open) {
                if (node.Valid()) {
                    for (auto child : node.GetChilds()) {
                        ShowNode(child);
                    }
                }
                ImGui::TreePop();
            }
            ImGui::PopID();
        }
        if (node.IsEntity()) {
            ImGui::PushID(node.GetID());
            auto        entity      = node.GetEntity();
            std::string name        = "        " ICON_MD_CHECK_BOX_OUTLINE_BLANK "  " + entity.Get<TagComponent>().tag;
            bool        is_selected = (m_SelectedNode.IsEntity() ? m_SelectedNode.GetEntity() == entity : false);
            if (ImGui::Selectable(name.c_str(), is_selected, ImGuiSelectableFlags_AllowDoubleClick | ImGuiSelectableFlags_SpanAllColumns) &&
                ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
                m_SelectedNode = node;
            }
            if (ImGui::BeginPopupContextItem()) {
                ShowNodeContextMenu(node);
                ImGui::EndPopup();
            }

            DropTarget(node);
            DragTarget(node);
            ImGui::PopID();
        }
    }

    void SceneHierarchyPanel::DragTarget(SceneHierarchy::Node node) {
        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 1.0f);
        if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceNoHoldToOpenOthers)) {
            m_From = node;
            ImGui::Text("%s", GetDNDText(node, m_To).c_str());
            ImGui::SetDragDropPayload("DND_HIERARCHY_NODE", &node, sizeof(node), ImGuiCond_Once);
            ImGui::EndDragDropSource();
        }
        ImGui::PopStyleVar();
    }
    void SceneHierarchyPanel::DropTarget(SceneHierarchy::Node node) {
        if (!PossibleDND(m_From, node)) {
            return;
        }
        if (ImGui::BeginDragDropTarget()) {
            m_To        = node;
            m_WasTarget = true;
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_HIERARCHY_NODE")) {
                auto from = *(SceneHierarchy::Node*)payload->Data;
                if (from.Valid() && m_To.IsFolder()) {
                    m_To.Attach(from);
                }
                if (from.IsEntity() && m_To.IsEntity()) {
                    auto parent = m_To.GetParent();
                    auto folder = parent.AddFolder("Folder");
                    folder.Attach(m_To);
                    folder.Attach(from);
                }
            }
            ImGui::EndDragDropTarget();
        }
    }
    bool SceneHierarchyPanel::PossibleDND(SceneHierarchy::Node from, SceneHierarchy::Node to) {
        if (!from.Valid() || !to.Valid()) {
            return false;
        }
        if (from.GetParent() == to) {
            return false;
        }
        if (to.IsEntity() && from.IsEntity()) {
            return true;
        }
        if (to.IsFolder() && from.Valid() && !from.IsAnsestorOf(to)) {
            return true;
        }
        return false;
    }
    std::string SceneHierarchyPanel::GetDNDText(SceneHierarchy::Node from, SceneHierarchy::Node to) {
        if (!PossibleDND(from, to)) {
            std::string message = "Moving ";
            if (from.IsEntity()) {
                message.append(from.GetEntity().Get<TagComponent>());
            }
            if (from.IsFolder()) {
                message.append(from.GetName());
            }
            return message;
        }
        if (to.IsEntity() && from.IsEntity()) {
            return "Create Folder with " + to.GetEntity().Get<TagComponent>().tag + " and " + from.GetEntity().Get<TagComponent>().tag;
        }
        if (to.IsFolder() && from.Valid()) {
            std::string message = "Moving ";
            if (from.IsEntity()) {
                message.append(from.GetEntity().Get<TagComponent>());
            }
            if (from.IsFolder()) {
                message.append(from.GetName());
            }
            message.append(" to ");
            message.append(to.GetName());
            return message;
        }
        return "Something went wrong";
    }
}  // namespace DE