#include "Panels/SceneHierarchyPanel.h"

namespace DE
{
    void SceneHierarchyPanel::SetActiveScene(Ref<Scene> scene) { m_Scene = scene; }
    void SceneHierarchyPanel::View()
    {
        DE_PROFILE_SCOPE("SceneHierarchyPanel View");

        if (m_Scene)
        {
            for (auto child : *m_Scene->GetHierarchy())
            {
                ShowNode(child);
            }
        }
    }

    void   SceneHierarchyPanel::UnSelect() { m_SelectedNode = nullptr; }
    Entity SceneHierarchyPanel::GetActiveEntity()
    {
        if (m_SelectedNode)
        {
            return m_SelectedNode->GetEntity();
        }
        return Entity();
    }

    void SceneHierarchyPanel::ShowNode(Ref<SceneHierarchyNode> node)
    {
        if (!node->IsEntity())
        {
            if (ImGui::TreeNode(node->GetName().c_str()))
            {
                for (auto child : *node)
                {
                    ShowNode(child);
                }
                ImGui::TreePop();
            }
        }
        else
        {
            auto entity = node->GetEntity();
            if (ImGui::Selectable(entity.GetComponent<TagComponent>().tag.c_str(), &node->Selected(), ImGuiSelectableFlags_AllowDoubleClick))
            {
                if (ImGui::IsMouseDoubleClicked(0))
                {
                    m_SelectedNode = node;
                }
            }
        }
    }
}  // namespace DE