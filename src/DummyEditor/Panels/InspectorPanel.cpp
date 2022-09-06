#include "Panels/InspectorPanel.h"

namespace DE
{
    void InspectorPanel::OnImGuiRender()
    {
        ImGui::Begin("Inspector");
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
                ImGui::CollapsingHeader("Transformation");
            }
            if (m_Entity.HasComponent<RenderMeshComponent>())
            {
                ImGui::CollapsingHeader("Render Mesh");
            }
            if (m_Entity.HasComponent<FPSCamera>())
            {
                ImGui::CollapsingHeader("FPSCamera");
            }
            if (m_Entity.HasComponent<LightSource>())
            {
                ImGui::CollapsingHeader("Light Soruce");
            }
        }
        ImGui::End();
    }
    void InspectorPanel::SetCurrentEntity(Entity entity) { m_Entity = entity; }

}  // namespace DE