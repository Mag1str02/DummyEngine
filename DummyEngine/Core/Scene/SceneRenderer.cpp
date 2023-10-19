#include "DummyEngine/Core/Scene/SceneRenderer.h"

#include "DummyEngine/Core/ECS/ECS.h"
#include "DummyEngine/Core/Objects/LightSources/LightSource.h"
#include "DummyEngine/Core/Rendering/Renderer/Renderer.h"
#include "DummyEngine/Core/Rendering/RendererOpenGL/GLDebug.h"
#include "DummyEngine/Core/ResourceManaging/ResourceManager.h"
#include "DummyEngine/Core/Scene/Components.h"
#include "DummyEngine/Core/Scripting/ScriptEngine.h"

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
        m_FrameBuffer->AddColorAttachment(Texture::Format::F32, Texture::Channels::RGBA);
        m_FrameBuffer->SetDepthAttachment(Texture::Format::F32);
    }
    void SceneRenderer::OnViewPortResize(U32 x, U32 y) {
        m_FrameBuffer->Resize(x, y);
    }
    void SceneRenderer::Render(Entity camera) {
        DE_PROFILE_SCOPE("Scene Render");
        Entity skybox;
        {
            DE_PROFILE_SCOPE("Shader Update");
            auto skyboxes = m_Scene->m_Storage->View<SkyBoxComponent>();
            if (!skyboxes.Empty()) {
                skybox = *skyboxes.begin();
            }
            UpdateShaders(camera.Get<FPSCamera>(), skybox);
        }
        {
            DE_PROFILE_SCOPE("Mesh Rendering");
            m_FrameBuffer->Bind();
            m_Lights->Bind(LIGHT_UB_ID);
            Renderer::SetViewport(m_FrameBuffer->GetWidth(), m_FrameBuffer->GetHeight());
            Renderer::Clear();
            if (skybox.Valid()) {
                Mat4 transform = Mat4(1.0);
                if (skybox.Has<TransformComponent>()) {
                    transform = skybox.Get<TransformComponent>().GetRotation();
                }
                static U32 cnt = 0;
                Renderer::Submit(skybox.Get<SkyBoxComponent>()->GetMap(), camera.Get<FPSCamera>(), transform);
            }
            for (auto e : m_Scene->View<ScriptComponent>()) {
                auto& component = e.Get<ScriptComponent>();
                if (component.Valid()) {
                    component->OnRender();
                }
            }
            for (auto e : m_Scene->View<RenderMeshComponent, ShaderComponent>()) {
                auto shader = e.Get<ShaderComponent>().shader;
                auto mesh   = e.Get<RenderMeshComponent>().mesh;
                int  res    = (mesh->p_Animator ? 1 : 0);
                shader->Bind();
                shader->SetInt("u_Animated", res);
                if (mesh->p_Animator) {
                    mesh->p_Animator->SetMatricies(shader);
                }
                Mat4 transform(1.0f);
                if (e.Has<TransformComponent>()) {
                    transform = e.Get<TransformComponent>().GetTransform();
                }
                Renderer::Submit(mesh, shader, transform);
            }
            m_FrameBuffer->UnBind();
        }
        {
            DE_PROFILE_SCOPE("Post-Processing");
            if (settings.bloom) {
                DE_PROFILE_SCOPE("Bloom");
                Renderer::Bloom(m_FrameBuffer->GetColorAttachment(0),
                                settings.bloom_threshold,
                                settings.bloom_soft_threshold,
                                settings.bloom_radius,
                                settings.bloom_depth,
                                settings.bloom_strength);
            }
            if (settings.gamma_tone_mapping) {
                DE_PROFILE_SCOPE("Gamma & HDR");
                Renderer::GammeHDRCorrecion(m_FrameBuffer->GetColorAttachment(0), settings.exposure, settings.gamma);
            }
        }
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
                m_Shaders[shader_id]->SetUniformBlock("ub_Lights", LIGHT_UB_ID);
            } else {
                LOG_WARNING("SceneRenderer", "Shader (", shader_id, ") not found in ResourceManager");
            }
        }
    }
}  // namespace DE