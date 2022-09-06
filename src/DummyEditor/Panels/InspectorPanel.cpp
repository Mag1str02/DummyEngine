#include "Panels/InspectorPanel.h"

namespace DE
{
    void InspectorPanel::View()
    {
        DE_PROFILE_SCOPE("InspectorPanel View");

        float sensitivity = 0.1;

        if (m_Entity.Valid())
        {
            if (m_Entity.HasComponent<TagComponent>())
            {
                auto& component = m_Entity.GetComponent<TagComponent>();
                if (ImGui::CollapsingHeader("Tag"))
                {
                    ImGui::InputText("", &component.tag);
                }
            }
            if (m_Entity.HasComponent<IdComponent>())
            {
                auto& component = m_Entity.GetComponent<IdComponent>();
                if (ImGui::CollapsingHeader("UUID"))
                {
                    ImGui::Text(component.Hex().c_str());
                }
            }
            if (m_Entity.HasComponent<TransformComponent>())
            {
                if (ImGui::CollapsingHeader("Transformation"))
                {
                    auto& transform = m_Entity.GetComponent<TransformComponent>();
                    ImGui::DragFloat3("Traslation", &(transform.translation.x), sensitivity, 0, 0);
                    ImGui::DragFloat3("Scale", &(transform.scale.x), sensitivity, 0, 0);
                    ImGui::DragFloat3("Rotation", &(transform.rotation.x), sensitivity, 0, 0);
                }
            }
            if (m_Entity.HasComponent<RenderMeshComponent>())
            {
                if (ImGui::CollapsingHeader("Render Mesh"))
                {
                    auto& render_mesh = m_Entity.GetComponent<RenderMeshComponent>();
                    ImGui::Text(("Mesh UUID: " + render_mesh.id.Hex()).c_str());
                }
            }
            if (m_Entity.HasComponent<FPSCamera>())
            {
                if (ImGui::CollapsingHeader("FPSCamera"))
                {}
            }
            if (m_Entity.HasComponent<LightSource>())
            {
                if (ImGui::CollapsingHeader("Light Soruce"))
                {
                    auto& source = m_Entity.GetComponent<LightSource>();

                    ImGui::ColorEdit3("Ambient", &(source.ambient.x));
                    ImGui::ColorEdit3("Diffuse", &(source.diffuse.x));
                    ImGui::ColorEdit3("Specular", &(source.specular.x));
                    ImGui::SliderFloat3("CLQ", &(source.clq.x), 0, 1);
                }
            }
        }
    }
    void InspectorPanel::SetActiveEntity(Entity entity) { m_Entity = entity; }

}  // namespace DE