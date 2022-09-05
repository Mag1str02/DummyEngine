#include "Panels/SceneHierarchyPanel.h"

namespace DE
{

    void SceneHierarchyPanel::SetActiveScene(Ref<Scene> scene) { m_Scene = scene; }
    void SceneHierarchyPanel::OnImGuiRender()
    {
        ImGui::Begin("Scene Hierarchy");
        if (m_Scene)
        {
            std::vector<Entity> entities = m_Scene->GetAllEntities();
            for (auto& entity : entities)
            {
                if (ImGui::TreeNode(entity.GetComponent<TagComponent>().tag.c_str()))
                {
                    if (entity.HasComponent<IdComponent>())
                    {
                        ImGui::Text(("UUID: " + std::to_string(entity.GetComponent<IdComponent>())).c_str());
                    }
                    if (entity.HasComponent<TransformComponent>())
                    {
                        ImGui::Text("Transform");
                    }
                    if (entity.HasComponent<ShaderComponent>())
                    {
                        ImGui::Text("Shader");
                    }
                    if (entity.HasComponent<RenderMeshComponent>())
                    {
                        ImGui::Text("Render Mesh");
                    }
                    if (entity.HasComponent<LightSource>())
                    {
                        ImGui::Text("Light Source");
                    }
                    ImGui::TreePop();
                }
            }
        }
        ImGui::End();
    }
}  // namespace DE