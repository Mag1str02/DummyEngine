#include "DummyEngine/Core/Objects/LightSources/LightManager.h"
#include "DummyEngine/ToolBox/Dev/FrameTimeReader.h"

namespace DE
{

    void LightManager::ActivatePointLight(Ref<Shader> shader, const PointLight& point_light, int id)
    {
        shader->SetFloat3("u_PointLights[" + std::to_string(id) + "].light.ambient", point_light.ambient);
        shader->SetFloat3("u_PointLights[" + std::to_string(id) + "].light.diffuse", point_light.diffuse);
        shader->SetFloat3("u_PointLights[" + std::to_string(id) + "].light.specular", point_light.specular);
        shader->SetFloat3("u_PointLights[" + std::to_string(id) + "].position", point_light.position);
        shader->SetFloat3("u_PointLights[" + std::to_string(id) + "].clq", point_light.clq);
    }
    void LightManager::ActivateDirectionalLight(Ref<Shader> shader, const DirectionalLight& directional_light, int id)
    {
        shader->SetFloat3("u_DirLights[" + std::to_string(id) + "].light.ambient", directional_light.ambient);
        shader->SetFloat3("u_DirLights[" + std::to_string(id) + "].light.diffuse", directional_light.diffuse);
        shader->SetFloat3("u_DirLights[" + std::to_string(id) + "].light.specular", directional_light.specular);
        shader->SetFloat3("u_DirLights[" + std::to_string(id) + "].direction", directional_light.direction);
    }
    void LightManager::ActivateSpotLight(Ref<Shader> shader, const SpotLight& spot_light, int id)
    {
        shader->SetFloat3("u_SpotLights[" + std::to_string(id) + "].point_light.light.ambient", spot_light.ambient);
        shader->SetFloat3("u_SpotLights[" + std::to_string(id) + "].point_light.light.diffuse", spot_light.diffuse);
        shader->SetFloat3("u_SpotLights[" + std::to_string(id) + "].point_light.light.specular", spot_light.specular);
        shader->SetFloat3("u_SpotLights[" + std::to_string(id) + "].point_light.position", spot_light.position);
        shader->SetFloat3("u_SpotLights[" + std::to_string(id) + "].point_light.clq", spot_light.clq);
        shader->SetFloat3("u_SpotLights[" + std::to_string(id) + "].direction", spot_light.direction);
        shader->SetFloat("u_SpotLights[" + std::to_string(id) + "].outer_cone_cos", spot_light.outer_cone_cos);
        shader->SetFloat("u_SpotLights[" + std::to_string(id) + "].inner_cone_cos", spot_light.inner_cone_cos);
    }

    LightManager::LightManager() {}
    std::string LightManager::GetName() const
    {
        return "Light Passing System";
    }
    void LightManager::Update(double dt)
    {
        auto& shaders = GetComponentArray<UniqueShader>();
        auto& u_PointLights = GetComponentArray<PointLight>();
        auto& u_DirLights = GetComponentArray<DirectionalLight>();
        auto& u_SpotLights = GetComponentArray<SpotLight>();
        for (auto [entity_id, unique_shader] : shaders)
        {
            auto& shader = unique_shader.shader;
            shader->Bind();
            int cnt_dir_light = 0;
            int cnt_point_light = 0;
            int cnt_spot_light = 0;
            for (auto [entity_id, directional_light] : u_DirLights)
            {
                ActivateDirectionalLight(shader, directional_light, cnt_dir_light++);
            }
            for (auto [entity_id, point_light] : u_PointLights)
            {
                ActivatePointLight(shader, point_light, cnt_point_light++);
            }
            for (auto [entity_id, spot_light] : u_SpotLights)
            {
                ActivateSpotLight(shader, spot_light, cnt_spot_light++);
            }
            shader->SetInt3("u_LightAmount", cnt_dir_light, cnt_point_light, cnt_spot_light);
        }
    }
}  // namespace DE