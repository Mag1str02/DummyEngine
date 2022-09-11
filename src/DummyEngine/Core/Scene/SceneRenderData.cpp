#include "Core/Scene/SceneRenderData.h"
#include "Core/Objects/LightSources/LightSource.h"
#include "Core/Rendering/Renderer/Renderer.h"
#include "Core/ResourceManaging/ResourceManager.h"
#include "Core/Scene/Components.h"
#include "Core/ECS/Entity.hpp"

namespace DE
{
    const uint32_t MAX_LIGHTS_IN_SCENE      = 1000;
    const uint32_t MAX_INSTANCES_PER_BUFFER = 1000;

    const uint32_t LIGHT_UNIFORM_BUFFER = 1;

    SceneRenderData::SceneRenderData(Scene* scene) : m_Scene(scene)
    {
        m_Lights = UniformBuffer::Create({BufferElementType::Float3,
                                          BufferElementType::Float3,
                                          BufferElementType::Float3,
                                          BufferElementType::Float3,
                                          BufferElementType::Float3,
                                          BufferElementType::Float3,
                                          BufferElementType::Float3},
                                         MAX_LIGHTS_IN_SCENE);
        m_Lights->Bind(LIGHT_UNIFORM_BUFFER);
    }
    void SceneRenderData::Render()
    {
        DE_PROFILE_SCOPE("Scene Render");

        auto& camera = m_Scene->GetCamera();

        auto meshes          = m_Scene->m_Storage.GetComponentArray<RenderMeshComponent>();
        auto shaders         = m_Scene->m_Storage.GetComponentArray<ShaderComponent>();
        auto transformations = m_Scene->m_Storage.GetComponentArray<TransformComponent>();
        auto skyboxes        = m_Scene->m_Storage.GetComponentArray<SkyBox>();

        Renderer::Clear();

        for (auto [id, shader] : m_Shaders)
        {
            shader->Bind();
            shader->SetMat4("u_ViewProjection", camera.GetViewProjection());
            shader->SetFloat3("u_CameraPos", camera.GetPos());
            // shader->SetUnifromBlock("ub_Lights", LIGHT_UNIFORM_BUFFER);
            glCheckError();
        }

        UpdateLights();

        for (auto& [ids, target] : m_InstancedMeshes)
        {
            target.first->UpdateInstanceBuffer();
            Renderer::Submit(target.first, target.second);
        }

        for (auto [id, skybox] : skyboxes)
        {
            transformations[id].translation = m_Scene->GetByName("player").GetComponent<FPSCamera>().GetPos();
            Renderer::Submit(skybox.map, shaders[id].shader, transformations[id].GetTransform());
        }
        glCheckError();
    }
    void SceneRenderData::UpdateLights()
    {
        DE_PROFILE_SCOPE("UpdateLights");

        auto& u_LightSources = m_Scene->m_Storage.GetComponentArray<LightSource>();

        int cnt_light_sources = 0;
        for (auto [entity_id, light_source] : u_LightSources)
        {
            m_Lights->at(cnt_light_sources).Get<Vec3>(0) = light_source.ambient;
            m_Lights->at(cnt_light_sources).Get<Vec3>(1) = light_source.diffuse;
            m_Lights->at(cnt_light_sources).Get<Vec3>(2) = light_source.specular;
            m_Lights->at(cnt_light_sources).Get<Vec3>(3) = light_source.direction;
            m_Lights->at(cnt_light_sources).Get<Vec3>(4) = light_source.position;
            m_Lights->at(cnt_light_sources).Get<Vec3>(5) = light_source.clq;
            m_Lights->at(cnt_light_sources).Get<Vec3>(6) = Vec3(light_source.outer_cone_cos, light_source.inner_cone_cos, LightSourceTypeToId(light_source.type));
            cnt_light_sources++;
        }
        m_Lights->PushData();
        for (auto [id, shader] : m_Shaders)
        {
            shader->SetInt("u_LightAmount", cnt_light_sources);
        }
        glCheckError();
    }
    void SceneRenderData::RequestShader(UUID shader_id)
    {
        if (!m_Shaders.contains(shader_id))
        {
            m_Shaders[shader_id] = ResourceManager::GetResource<Shader>(shader_id);
        }
    }

    Ref<RenderMeshInstance> SceneRenderData::GetRenderMeshInstance(UUID mesh_id, UUID shader_id)
    {
        if (!m_InstancedMeshes.contains({mesh_id, shader_id}))
        {
            CreateInstancedMesh(mesh_id, shader_id);
        }
        Ref<RenderMeshInstance> instance = CreateRef<RenderMeshInstance>();
        instance->Bind(m_InstancedMeshes[{mesh_id, shader_id}].first);
        return instance;
    }

    void SceneRenderData::CreateInstancedMesh(UUID mesh_id, UUID shader_id)
    {
        m_InstancedMeshes[{mesh_id, shader_id}] = {ResourceManager::GetResource<RenderMesh>(mesh_id)->Copy(), ResourceManager::GetResource<Shader>(shader_id)};
        m_InstancedMeshes[{mesh_id, shader_id}].first->SetInstanceBuffer({{BufferElementType::Mat4}, 1}, MAX_INSTANCES_PER_BUFFER);
        RequestShader(shader_id);
    }
}  // namespace DE