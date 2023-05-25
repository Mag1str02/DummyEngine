#include "DummyEngine/Core/Scene/SceneRenderer.h"

#include "DummyEngine/Core/ECS/ECS.h"
#include "DummyEngine/Core/Objects/LightSources/LightSource.h"
#include "DummyEngine/Core/Rendering/Renderer/Renderer.h"
#include "DummyEngine/Core/Rendering/RendererOpenGL/GLDebug.h"
#include "DummyEngine/Core/ResourceManaging/ResourceManager.h"
#include "DummyEngine/Core/Scene/Components.h"

namespace DE {
    const U32 MAX_LIGHTS_IN_SCENE      = 1000;
    const U32 MAX_INSTANCES_PER_BUFFER = 1000;

    const U32 LIGHT_UB_ID = 1;

    SceneRenderer::SceneRenderer(Scene* scene) : m_Scene(scene) {
        m_Lights      = UniformBuffer::Create({BufferElementType::Float3,
                                               BufferElementType::Float3,
                                               BufferElementType::Float3,
                                               BufferElementType::Float3,
                                               BufferElementType::Float3,
                                               BufferElementType::Float3,
                                               BufferElementType::Float3},
                                         MAX_LIGHTS_IN_SCENE);
        m_FrameBuffer = FrameBuffer::Create({1920, 1080});
        m_FrameBuffer->AddColorAttachment(TextureChannels::RGBA);
        m_FrameBuffer->SetDepthAttachment(TextureChannels::Depth);
    }
    void SceneRenderer::OnViewPortResize(U32 x, U32 y) {
        m_FrameBuffer->Resize(x, y);
    }
    void SceneRenderer::Render(Entity camera) {
        DE_PROFILE_SCOPE("Scene Render");
        auto& scene_camera = camera.Get<FPSCamera>();
        m_FrameBuffer->Bind();
        m_Lights->Bind(LIGHT_UB_ID);
        Renderer::SetViewport(m_FrameBuffer->GetWidth(), m_FrameBuffer->GetHeight());
        Renderer::Clear();

        Entity skybox;
        auto   skyboxes = m_Scene->m_Storage->View<SkyBoxComponent>();
        if (!skyboxes.Empty()) {
            skybox = *skyboxes.begin();
        }
        UpdateShaders(scene_camera, skybox);

        if (skybox.Valid()) {
            Mat4 transform = Mat4(1.0);
            if (skybox.Has<TransformComponent>()) {
                transform = skybox.Get<TransformComponent>().GetRotation();
            }
            Renderer::Submit(skybox.Get<SkyBoxComponent>()->GetMap(), scene_camera, transform);
        }

        for (auto& [ids, target] : m_InstancedMeshes) {
            target.first->UpdateInstanceBuffer();
            int res = (target.first->p_Animator ? 1 : 0);
            target.second->Bind();
            target.second->SetInt("u_Animated", res);
            if (target.first->p_Animator) {
                target.first->p_Animator->SetMatricies(target.second);
            }
            Renderer::Submit(target.first, target.second);
        }
        m_FrameBuffer->UnBind();
    }

    void SceneRenderer::UpdateShaders(const FPSCamera& camera, Entity skybox) {
        DE_PROFILE_SCOPE("UpdateShaders");

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
            if (skybox.Valid() && skybox.Has<SkyBoxComponent>()) {
                skybox.Get<SkyBoxComponent>()->ApplyIBL(shader);
                if (skybox.Has<TransformComponent>()) {
                    auto& transform   = skybox.Get<TransformComponent>();
                    Vec3  rotation    = -(transform.rotation + transform.rotation_offet);
                    Mat4  mt_rotation = glm::toMat4(glm::quat(glm::radians(rotation)));
                    shader->SetMat4("u_EnvRotation", mt_rotation);
                }
            }
            shader->SetInt("u_LightAmount", cnt_light_sources);
            shader->SetMat4("u_Camera.view", camera.GetViewMatrix());
            shader->SetMat4("u_Camera.projection", camera.GetProjectionMatrix());
        }
    }

    Ref<FrameBuffer> SceneRenderer::GetFrameBuffer() {
        return m_FrameBuffer;
    }

    void SceneRenderer::RequestShader(UUID shader_id) {
        if (!m_Shaders.contains(shader_id)) {
            auto shader = ResourceManager::GetShader(shader_id);
            if (shader) {
                m_Shaders[shader_id] = shader.value();
                m_Shaders[shader_id]->SetUnifromBlock("ub_Lights", LIGHT_UB_ID);
            } else {
                LOG_WARNING("SceneRenderer", "Shader (", shader_id, ") not found in ResourceManager");
            }
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