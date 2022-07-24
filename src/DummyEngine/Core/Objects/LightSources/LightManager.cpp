#include "DummyEngine/Core/Objects/LightSources/LightManager.h"
#include "DummyEngine/ToolBox/Dev/FrameTimeReader.h"

namespace DE
{

    void LightManager::ActivatePointLight(Ref<Shader> shader, const PointLight& point_light, int id)
    {
        shader->SetFloat3("point_lights[" + std::to_string(id) + "].light.ambient", point_light.ambient);
        shader->SetFloat3("point_lights[" + std::to_string(id) + "].light.diffuse", point_light.diffuse);
        shader->SetFloat3("point_lights[" + std::to_string(id) + "].light.specular", point_light.specular);
        shader->SetFloat3("point_lights[" + std::to_string(id) + "].position", point_light.position);
        shader->SetFloat3("point_lights[" + std::to_string(id) + "].clq", point_light.clq);
    }
    void LightManager::ActivateDirectionalLight(Ref<Shader> shader, const DirectionalLight& directional_light, int id)
    {
        shader->SetFloat3("directional_lights[" + std::to_string(id) + "].light.ambient", directional_light.ambient);
        shader->SetFloat3("directional_lights[" + std::to_string(id) + "].light.diffuse", directional_light.diffuse);
        shader->SetFloat3("directional_lights[" + std::to_string(id) + "].light.specular", directional_light.specular);
        shader->SetFloat3("directional_lights[" + std::to_string(id) + "].direction", directional_light.direction);
    }
    void LightManager::ActivateSpotLight(Ref<Shader> shader, const SpotLight& spot_light, int id)
    {
        shader->SetFloat3("spot_lights[" + std::to_string(id) + "].point_light.light.ambient", spot_light.ambient);
        shader->SetFloat3("spot_lights[" + std::to_string(id) + "].point_light.light.diffuse", spot_light.diffuse);
        shader->SetFloat3("spot_lights[" + std::to_string(id) + "].point_light.light.specular", spot_light.specular);
        shader->SetFloat3("spot_lights[" + std::to_string(id) + "].point_light.position", spot_light.position);
        shader->SetFloat3("spot_lights[" + std::to_string(id) + "].point_light.clq", spot_light.clq);
        shader->SetFloat3("spot_lights[" + std::to_string(id) + "].direction", spot_light.direction);
        shader->SetFloat("spot_lights[" + std::to_string(id) + "].outer_cone_cos", spot_light.outer_cone_cos);
        shader->SetFloat("spot_lights[" + std::to_string(id) + "].inner_cone_cos", spot_light.inner_cone_cos);
    }

    LightManager::LightManager() {}
    std::string LightManager::GetName() const
    {
        return "Light Passing System";
    }
    void LightManager::Update(double dt)
    {
        auto& shaders = GetComponentArray<UniqueShader>();
        auto& point_lights = GetComponentArray<PointLight>();
        auto& directional_lights = GetComponentArray<DirectionalLight>();
        auto& spot_lights = GetComponentArray<SpotLight>();
        for (auto [entity_id, unique_shader] : shaders)
        {
            auto& shader = unique_shader.shader;
            shader->Bind();
            int cnt_dir_light = 0;
            int cnt_point_light = 0;
            int cnt_spot_light = 0;
            for (auto [entity_id, directional_light] : directional_lights)
            {
                ActivateDirectionalLight(shader, directional_light, cnt_dir_light++);
            }
            for (auto [entity_id, point_light] : point_lights)
            {
                ActivatePointLight(shader, point_light, cnt_point_light++);
            }
            for (auto [entity_id, spot_light] : spot_lights)
            {
                ActivateSpotLight(shader, spot_light, cnt_spot_light++);
            }
            shader->SetInt3("light_amount", cnt_dir_light, cnt_point_light, cnt_spot_light);
        }
    }
}  // namespace DE