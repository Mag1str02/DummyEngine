#include "SceneHierarchyPanel.h"

#include "DummyEditor/ImGuiUtils/ImGuiIcons.h"
#include "DummyEditor/ImGuiUtils/ImGuiUtils.h"

#include "DummyEngine/Core/Scene/Components.h"
#include "DummyEngine/Utils/Debug/Profiler.h"

#include <imgui.h>
#include <imgui_stdlib.h>

namespace DummyEngine {

    void SceneHierarchyPanel::SetActiveScene(WeakRef<Scene> scene) {
        scene_         = scene;
        selected_node_ = SceneHierarchy::Node();
    }
    void SceneHierarchyPanel::OnImGui() {
        DE_PROFILE_SCOPE("SceneHierarchyPanel OnImGui");
        if (m_Controller) {
            if (ImGui::Begin(ICON_MD_ACCOUNT_TREE "  Scene Hierarchy")) {
                auto scene = scene_.lock();
                if (scene) {
                    if (!was_target_) {
                        to_ = SceneHierarchy::Node();
                    }
                    was_target_ = false;
                    ShowNode(scene->GetHierarchyRoot());
                }
            }
            ImGui::End();
        }
    }

    Entity SceneHierarchyPanel::GetActiveEntity() {
        auto scene = scene_.lock();
        if (scene && selected_node_.IsEntity()) {
            return selected_node_.GetEntity();
        }
        return Entity();
    }

    void SceneHierarchyPanel::ShowNodeContextMenu(SceneHierarchy::Node node) {
        if (node.IsFolder()) {
            if (ImGui::MenuItem("Create Entity")) {
                auto entity = scene_.lock()->CreateEntity("Entity", false);
                node.AddEntity(entity);
                ImGui::CloseCurrentPopup();
            }
            if (ImGui::MenuItem("Create Folder")) {
                node.AddFolder("Folder");
                ImGui::CloseCurrentPopup();
            }
            if (ImGui::MenuItem("Rename")) {
                rename_ = node;
                ImGui::CloseCurrentPopup();
            }
            if (node.GetID() != 0 && ImGui::MenuItem("Delete")) {
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
                auto entity = scene_.lock()->CloneEntity(node.GetEntity());
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
            ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen;
            if (node != rename_) {
                flags |= ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_SpanAvailWidth;
            }
            ImGuiUtils::ScopedID id(node.GetID());
            open = ImGui::TreeNodeEx("Node", flags, ICON_MD_FOLDER);
            if (ImGui::BeginPopupContextItem("Node")) {
                ShowNodeContextMenu(node);
                ImGui::EndPopup();
            }
            if (node.Valid()) {
                DropTarget(node);
                DragTarget(node);
                ImGui::SameLine();
                if (rename_ == node) {
                    ImGui::SetKeyboardFocusHere();
                    if (ImGui::InputText(
                            "###FolderRename", &rename_.GetName(), ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue)) {
                        rename_ = SceneHierarchy::Node();
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
        }
        if (node.IsEntity()) {
            ImGuiUtils::ScopedID id(node.GetID());
            auto                 entity      = node.GetEntity();
            std::string          name        = "        " ICON_MD_CHECK_BOX_OUTLINE_BLANK "  " + entity.Get<TagComponent>().Tag;
            bool                 is_selected = (selected_node_.IsEntity() ? selected_node_.GetEntity() == entity : false);
            if (ImGui::Selectable(name.c_str(), is_selected, ImGuiSelectableFlags_AllowDoubleClick | ImGuiSelectableFlags_SpanAllColumns) &&
                ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
                selected_node_ = node;
            }
            if (ImGui::BeginPopupContextItem()) {
                ShowNodeContextMenu(node);
                ImGui::EndPopup();
            }

            DropTarget(node);
            DragTarget(node);
        }
    }

    void SceneHierarchyPanel::DragTarget(SceneHierarchy::Node node) {
        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 1.0f);
        if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceNoHoldToOpenOthers)) {
            from_ = node;
            ImGui::Text("%s", GetDNDText(node, to_).c_str());
            ImGui::SetDragDropPayload("DND_HIERARCHY_NODE", &node, sizeof(node), ImGuiCond_Once);
            ImGui::EndDragDropSource();
        }
        ImGui::PopStyleVar();
    }
    void SceneHierarchyPanel::DropTarget(SceneHierarchy::Node node) {
        if (!PossibleDND(from_, node)) {
            return;
        }
        if (ImGui::BeginDragDropTarget()) {
            to_         = node;
            was_target_ = true;
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_HIERARCHY_NODE")) {
                auto from = *(SceneHierarchy::Node*)payload->Data;
                if (from.Valid() && to_.IsFolder()) {
                    to_.Attach(from);
                }
                if (from.IsEntity() && to_.IsEntity()) {
                    auto parent = to_.GetParent();
                    auto folder = parent.AddFolder("Folder");
                    folder.Attach(to_);
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
            return "Create Folder with " + to.GetEntity().Get<TagComponent>().Tag + " and " + from.GetEntity().Get<TagComponent>().Tag;
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

}  // namespace DummyEngine