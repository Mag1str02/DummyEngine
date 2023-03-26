#include "DummyEngine/Core/Scene/SceneRenderData.h"
#include "DummyEngine/Core/Objects/LightSources/LightSource.h"
#include "DummyEngine/Core/Rendering/Renderer/Renderer.h"
#include "DummyEngine/Core/ResourceManaging/ResourceManager.h"
#include "DummyEngine/Core/Scene/Components.h"
#include "DummyEngine/Core/ECS/Entity.hpp"

namespace DE
{
    const uint32_t MAX_LIGHTS_IN_SCENE      = 1000;
    const uint32_t MAX_VP_AMOUNT            = 32;
    const uint32_t MAX_INSTANCES_PER_BUFFER = 1000;

    const uint32_t VP_UB_ID    = 1;
    const uint32_t LIGHT_UB_ID = 2;

    SceneRenderData::SceneRenderData(Scene* scene) : m_Scene(scene)
    {
        m_VP     = UniformBuffer::Create({BufferElementType::Mat4, BufferElementType::Mat4}, MAX_VP_AMOUNT);
        m_Lights = UniformBuffer::Create({BufferElementType::Float3,
                                          BufferElementType::Float3,
                                          BufferElementType::Float3,
                                          BufferElementType::Float3,
                                          BufferElementType::Float3,
                                          BufferElementType::Float3,
                                          BufferElementType::Float3},
                                         MAX_LIGHTS_IN_SCENE);
        m_VP->Bind(VP_UB_ID);
        m_Lights->Bind(LIGHT_UB_ID);
    }
    void SceneRenderData::Render()
    {
        DE_PROFILE_SCOPE("Scene Render");

        auto camera = m_Scene->GetCamera();

        auto meshes          = m_Scene->m_Storage.GetComponentArray<RenderMeshComponent>();
        auto shaders         = m_Scene->m_Storage.GetComponentArray<ShaderComponent>();
        auto transformations = m_Scene->m_Storage.GetComponentArray<TransformComponent>();
        auto skyboxes        = m_Scene->m_Storage.GetComponentArray<SkyBox>();

        Renderer::Clear();

        UpdateVP();
        UpdateLights();

        SetVPEntity(camera);

        for (auto& [ids, target] : m_InstancedMeshes)
        {
            target.first->UpdateInstanceBuffer();
            Renderer::Submit(target.first, target.second);
        }

        for (auto [id, skybox] : skyboxes)
        {
            transformations[id].translation = m_Camera.GetComponent<FPSCamera>().GetPos();
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
            m_Lights->at(cnt_light_sources).Get<Vec3>(6) =
                Vec3(light_source.outer_cone_cos, light_source.inner_cone_cos, LightSourceTypeToId(light_source.type));
            cnt_light_sources++;
        }
        m_Lights->PushData();
        for (auto [id, shader] : m_Shaders)
        {
            shader->Bind();
            shader->Bind();
            shader->SetInt("u_LightAmount", cnt_light_sources);
        }
        glCheckError();
    }
    void SceneRenderData::UpdateVP()
    {
        DE_PROFILE_SCOPE("UpdateVP");

        for (auto [entity_id, id] : m_EntityToVPIndex)
        {
            Entity entity(entity_id, m_Scene);
            if (entity.HasComponent<FPSCamera>())
            {
                auto& camera              = entity.GetComponent<FPSCamera>();
                m_VP->at(id).Get<Mat4>(0) = camera.GetViewMatrix();
                m_VP->at(id).Get<Mat4>(1) = camera.GetProjectionMatrix();
            }
        }
        m_VP->PushData();
        glCheckError();
    }

    void SceneRenderData::SetVPEntity(const Entity& entity)
    {
        DE_ASSERT(m_EntityToVPIndex.contains(entity.m_Id), "Wrong VP entity.");
        for (auto [id, shader] : m_Shaders)
        {
            shader->Bind();
            shader->SetInt("u_VP", m_EntityToVPIndex[entity.m_Id]);
            glCheckError();
        }
    }

    void SceneRenderData::RequestShader(UUID shader_id)
    {
        if (!m_Shaders.contains(shader_id))
        {
            m_Shaders[shader_id] = ResourceManager::GetResource<Shader>(shader_id);
            m_Shaders[shader_id]->SetUnifromBlock("ub_Lights", LIGHT_UB_ID);
            m_Shaders[shader_id]->SetUnifromBlock("ub_VP", VP_UB_ID);
        }
    }
    void SceneRenderData::AddVPEntity(const Entity& entity)
    {
        if (!m_EntityToVPIndex.contains(entity.m_Id))
        {
            uint32_t index                 = m_EntityToVPIndex.size();
            m_EntityToVPIndex[entity.m_Id] = index;
        }
    }
    void SceneRenderData::SetCamera(const Entity& camera) { m_Camera = camera; }

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
        m_InstancedMeshes[{mesh_id, shader_id}] = {ResourceManager::GetResource<RenderMesh>(mesh_id)->Copy(),
                                                   ResourceManager::GetResource<Shader>(shader_id)};
        m_InstancedMeshes[{mesh_id, shader_id}].first->SetInstanceBuffer({{BufferElementType::Mat4}, 1}, MAX_INSTANCES_PER_BUFFER);
        RequestShader(shader_id);
    }
}  // namespace DE