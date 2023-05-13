#include "DummyEngine/Core/Scene/SceneRenderer.h"

#include "DummyEngine/Core/ECS/ECS.h"
#include "DummyEngine/Core/Objects/LightSources/LightSource.h"
#include "DummyEngine/Core/Rendering/Renderer/Renderer.h"
#include "DummyEngine/Core/Rendering/RendererOpenGL/GLDebug.h"
#include "DummyEngine/Core/ResourceManaging/ResourceManager.h"
#include "DummyEngine/Core/Scene/Components.h"

namespace DE {
    const U32 MAX_LIGHTS_IN_SCENE      = 1000;
    const U32 MAX_VP_AMOUNT            = 32;
    const U32 MAX_INSTANCES_PER_BUFFER = 1000;

    const U32 VP_UB_ID    = 1;
    const U32 LIGHT_UB_ID = 2;

    SceneRenderer::SceneRenderer(Scene* scene) : m_Scene(scene) {
        m_VP          = UniformBuffer::Create({BufferElementType::Mat4, BufferElementType::Mat4}, MAX_VP_AMOUNT);
        m_Lights      = UniformBuffer::Create({BufferElementType::Float3,
                                               BufferElementType::Float3,
                                               BufferElementType::Float3,
                                               BufferElementType::Float3,
                                               BufferElementType::Float3,
                                               BufferElementType::Float3,
                                               BufferElementType::Float3},
                                         MAX_LIGHTS_IN_SCENE);
        m_FrameBuffer = FrameBuffer::Create({1920, 1080});
        m_FrameBuffer->AddColorAttachment(TextureFormat::RGBA);
        m_FrameBuffer->SetDepthAttachment(TextureFormat::Depth);
    }
    void SceneRenderer::OnViewPortResize(U32 x, U32 y) {
        m_FrameBuffer->Resize(x, y);
    }
    void SceneRenderer::Render(Entity camera) {
        DE_PROFILE_SCOPE("Scene Render");

        m_FrameBuffer->Bind();
        m_VP->Bind(VP_UB_ID);
        m_Lights->Bind(LIGHT_UB_ID);
        Renderer::OnWindowResize(m_FrameBuffer->GetWidth(), m_FrameBuffer->GetHeight());
        Renderer::Clear();

        UpdateVP();
        UpdateLights();

        SetVPEntity(camera);

        for (auto& [ids, target] : m_InstancedMeshes) {
            target.first->UpdateInstanceBuffer();
            Renderer::Submit(target.first, target.second);
        }

        for (auto e : m_Scene->m_Storage->View<SkyBox, TransformComponent, ShaderComponent>()) {
            e.Get<TransformComponent>().translation = camera.Get<FPSCamera>().GetPos();
            Renderer::Submit(e.Get<SkyBox>().map, e.Get<ShaderComponent>().shader, e.Get<TransformComponent>().GetTransform());
        }
        m_FrameBuffer->UnBind();
    }

    void SceneRenderer::UpdateLights() {
        DE_PROFILE_SCOPE("UpdateLights");

        int cnt_light_sources = 0;
        for (auto enitity : m_Scene->m_Storage->View<LightSource>()) {
            auto& light_source                           = enitity.Get<LightSource>();
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
        for (auto [id, shader] : m_Shaders) {
            shader->Bind();
            shader->Bind();
            shader->SetInt("u_LightAmount", cnt_light_sources);
        }
    }
    void SceneRenderer::UpdateVP() {
        DE_PROFILE_SCOPE("UpdateVP");

        for (std::pair<Entity, U32> p : m_EntityToVPIndex) {
            Entity entity = p.first;
            U32    id     = p.second;
            if (entity.Has<FPSCamera>()) {
                auto& camera              = entity.Get<FPSCamera>();
                m_VP->at(id).Get<Mat4>(0) = camera.GetViewMatrix();
                m_VP->at(id).Get<Mat4>(1) = camera.GetProjectionMatrix();
            }
        }
        m_VP->PushData();
    }

    Ref<FrameBuffer> SceneRenderer::GetFrameBuffer() {
        return m_FrameBuffer;
    }

    void SceneRenderer::SetVPEntity(const Entity& entity) {
        DE_ASSERT(m_EntityToVPIndex.contains(entity), "Wrong VP entity");
        for (auto [id, shader] : m_Shaders) {
            shader->Bind();
            shader->SetInt("u_VP", m_EntityToVPIndex[entity]);
        }
    }

    void SceneRenderer::RequestShader(UUID shader_id) {
        if (!m_Shaders.contains(shader_id)) {
            auto shader = ResourceManager::GetShader(shader_id);
            if (shader) {
                m_Shaders[shader_id] = shader.value();
                m_Shaders[shader_id]->SetUnifromBlock("ub_Lights", LIGHT_UB_ID);
                m_Shaders[shader_id]->SetUnifromBlock("ub_VP", VP_UB_ID);
            } else {
                LOG_WARNING("SceneRenderer", "Shader (", shader_id, ") not found in ResourceManager");
            }
        }
    }
    void SceneRenderer::AddVPEntity(const Entity& entity) {
        if (!m_EntityToVPIndex.contains(entity)) {
            U32 index                 = m_EntityToVPIndex.size();
            m_EntityToVPIndex[entity] = index;
        }
    }

    Ref<RenderMeshInstance> SceneRenderer::GetRenderMeshInstance(UUID mesh_id, UUID shader_id) {
        if (!m_InstancedMeshes.contains({mesh_id, shader_id})) {
            CreateInstancedMesh(mesh_id, shader_id);
        }
        Ref<RenderMeshInstance> instance = CreateRef<RenderMeshInstance>();
        instance->Bind(m_InstancedMeshes[{mesh_id, shader_id}].first);
        return instance;
    }

    void SceneRenderer::CreateInstancedMesh(UUID mesh_id, UUID shader_id) {
        auto mesh   = ResourceManager::GetRenderMesh(mesh_id);
        auto shader = ResourceManager::GetShader(shader_id);
        if (!mesh) {
            LOG_WARNING("SceneRenderer", "RenderMesh (", mesh_id, ") not found in ResourceManager");
        } else if (!shader) {
            LOG_WARNING("SceneRenderer", "Shader (", shader_id, ") not found in ResourceManager");
        } else {
            m_InstancedMeshes[{mesh_id, shader_id}] = {mesh.value()->Copy(), shader.value()};
            m_InstancedMeshes[{mesh_id, shader_id}].first->SetInstanceBuffer({{BufferElementType::Mat4}, 1}, MAX_INSTANCES_PER_BUFFER);
            RequestShader(shader_id);
        }
    }
}  // namespace DE