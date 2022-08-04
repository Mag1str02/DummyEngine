#include "Core/Objects/LightSources/LightManager.h"
#include "ToolBox/Dev/FrameTimeReader.h"

namespace DE
{
    uint32_t LightSourceTypeToId(LightSourceType type)
    {
        switch (type)
        {
            case LightSourceType::Direction: return 1;
            case LightSourceType::Point: return 2;
            case LightSourceType::Spot: return 3;
            default: return 0;
        }
    }

    void LightManager::ActivateLightSource(Ref<Shader> shader, const LightSource& source, int id)
    {
        shader->SetFloat3("u_LightSources[" + std::to_string(id) + "].m_Ambient", source.ambient);
        shader->SetFloat3("u_LightSources[" + std::to_string(id) + "].m_Diffuse", source.diffuse);
        shader->SetFloat3("u_LightSources[" + std::to_string(id) + "].m_Specular", source.specular);
        shader->SetFloat3("u_LightSources[" + std::to_string(id) + "].m_Position", source.position);
        shader->SetFloat3("u_LightSources[" + std::to_string(id) + "].m_CLQ", source.clq);
        shader->SetFloat3("u_LightSources[" + std::to_string(id) + "].m_Direction", source.direction);
        shader->SetFloat3("u_LightSources[" + std::to_string(id) + "].m_ConesAndType",
                          source.outer_cone_cos,
                          source.inner_cone_cos,
                          LightSourceTypeToId(source.type));
    }

    LightManager::LightManager() {}
    std::string LightManager::GetName() const
    {
        return "Light Passing System";
    }
    void LightManager::Update(double dt)
    {
        auto& shaders = GetComponentArray<UniqueShader>();
        auto& u_LightSources = GetComponentArray<LightSource>();
        for (auto [entity_id, unique_shader] : shaders)
        {
            auto& shader = unique_shader.shader;
            shader->Bind();
            int cnt_light_sources = 0;
            for (auto [entity_id, light_source] : u_LightSources)
            {
                ActivateLightSource(shader, light_source, cnt_light_sources++);
            }
            shader->SetInt("u_LightAmount", cnt_light_sources);
        }
    }
}  // namespace DE