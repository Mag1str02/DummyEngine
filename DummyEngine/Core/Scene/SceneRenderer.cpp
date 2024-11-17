#include "DummyEngine/Core/Scene/SceneRenderer.h"

#include "DummyEngine/Core/Console/Console.hpp"
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
        m_Lights = UniformBuffer::Create(
            {
                BufferElementType::Float3,
                BufferElementType::Float3,
                BufferElementType::Float3,
                BufferElementType::Float3,
                BufferElementType::Float3,
                BufferElementType::Float3,
                BufferElementType::Float3,
            },
            MAX_LIGHTS_IN_SCENE);
        m_FrameBuffer = FrameBuffer::Create({1920, 1080});
        m_FrameBuffer->AddColorAttachment(Texture::Format::F32, Texture::Channels::RGBA);
        m_FrameBuffer->SetDepthAttachment(Texture::Format::F32);

        if (settings.use_directional_shadow_map) {
            initializeShadowMap();
        }
    }
    void SceneRenderer::OnViewPortResize(U32 x, U32 y) {
        m_FrameBuffer->Resize(x, y);
    }
    void SceneRenderer::Render(Entity cam) {
        DE_PROFILE_SCOPE("Scene Render");
        Entity skybox;

        auto&     camera      = cam.Get<FPSCamera>();
        FPSCamera lightCamera = FPSCamera(Vec3{0, 100, 0}, Vec3{0, -1, 0});
        if (settings.use_directional_shadow_map) {
            DE_PROFILE_SCOPE("Directional ShadowMap Rendering");
            if (!m_ShadowMap) {
                initializeShadowMap();
            }
            m_ShadowMap->Bind();
            Renderer::Enable(RenderSetting::FaceCulling);
            Renderer::SetViewport(m_ShadowMap->GetWidth(), m_ShadowMap->GetHeight());
            Renderer::Clear();
            auto shader = Renderer::GetShader(Renderer::Shaders::DirectionalShadowMap);
            shader->Bind();

            for (auto entity : m_Scene->m_Storage->View<LightSource, TransformComponent>()) {
                if (entity.Get<LightSource>().type == LightSourceType::Direction) {
                    lightCamera.SetPos(entity.Get<TransformComponent>().translation);
                    lightCamera.SetDir(entity.Get<LightSource>().direction);
                }
            }
            shader->SetMat4("u_Camera.view", lightCamera.GetViewMatrix());
            shader->SetMat4("u_Camera.projection", lightCamera.GetOrthoProjectionMatrix());
            for (auto e : m_Scene->View<RenderMeshComponent>()) {
                auto mesh = e.Get<RenderMeshComponent>().mesh;
                int  res  = (mesh->p_Animator ? 1 : 0);
                shader->SetInt("u_Animated", res);
                if (mesh->p_Animator) {
                    mesh->p_Animator->SetMatricies(shader);
                }
                Mat4 transform(1.0f);
                if (e.Has<TransformComponent>()) {
                    transform = e.Get<TransformComponent>().GetTransform();
                }
                for (const auto& submesh : mesh->GetSubMeshes()) {
                    Renderer::Submit(submesh.vertex_array, shader, transform);
                }
            }
            Renderer::Disable(RenderSetting::FaceCulling);
            m_ShadowMap->UnBind();
        }
        if (settings.use_point_shadows) {
            DE_PROFILE_SCOPE("PointShadowMap Rendering");
            auto shader = Renderer::GetShader(Renderer::Shaders::OmnidirectionalShadowMap);
            shader->Bind();
            U32       size       = Console::GetInt("r_psm_size");
            glm::mat4 shadowProj = glm::perspective(glm::radians(90.f), 1.f, Console::GetFloat("r_psm_near"), Console::GetFloat("r_psm_far"));
            for (auto entity : m_Scene->m_Storage->View<LightSource>()) {
                auto  id          = entity.Get<IDComponent>().Get();
                auto& lightSource = entity.Get<LightSource>();
                if (lightSource.type != LightSourceType::Point) {
                    continue;
                }
                Ref<FrameBuffer> fbo;
                if (m_PointShadowCubemaps.find(id) == m_PointShadowCubemaps.end()) {
                    auto map = CubeMap::Create(size, Texture::Format::F32, Texture::Channels::Depth, false, true);
                    fbo      = FrameBuffer::Create({size, size});
                    fbo->SetDepthAttachment(map);
                    m_PointShadowCubemaps[id]     = map;
                    m_PointShadowFrameBuffers[id] = fbo;
                } else {
                    fbo = m_PointShadowFrameBuffers[id];
                }
                fbo->Bind();
                Renderer::Enable(RenderSetting::FaceCulling);
                Renderer::SetViewport(size, size);
                Renderer::Clear();
                auto                   pos = lightSource.position;
                std::vector<glm::mat4> shadowMatrices(6);
                shadowMatrices[0] = shadowProj * glm::lookAt(pos, pos + glm::vec3(1., 0., 0.), glm::vec3(0., -1., 0.));
                shadowMatrices[1] = shadowProj * glm::lookAt(pos, pos + glm::vec3(-1., 0., 0.), glm::vec3(0., -1., 0.));
                shadowMatrices[2] = shadowProj * glm::lookAt(pos, pos + glm::vec3(0., 1., 0.), glm::vec3(0., 0., 1.));
                shadowMatrices[3] = shadowProj * glm::lookAt(pos, pos + glm::vec3(0., -1., 0.), glm::vec3(0., 0., -1.));
                shadowMatrices[4] = shadowProj * glm::lookAt(pos, pos + glm::vec3(0., 0., 1.), glm::vec3(0., -1., 0.));
                shadowMatrices[5] = shadowProj * glm::lookAt(pos, pos + glm::vec3(0., 0., -1.), glm::vec3(0., -1., 0.));
                for (size_t i = 0; i < 6; i++) {
                    shader->SetMat4(StrCat("shadowMatrices", "[", i, "]"), shadowMatrices[i]);
                }
                shader->SetFloat("far_plane", Console::GetFloat("r_psm_far"));
                shader->SetFloat3("lightPos", pos);
                for (auto e : m_Scene->View<RenderMeshComponent>()) {
                    auto mesh = e.Get<RenderMeshComponent>().mesh;
                    int  res  = (mesh->p_Animator ? 1 : 0);
                    shader->SetInt("u_Animated", res);
                    if (mesh->p_Animator) {
                        mesh->p_Animator->SetMatricies(shader);
                    }
                    Mat4 transform(1.0f);
                    if (e.Has<TransformComponent>()) {
                        transform = e.Get<TransformComponent>().GetTransform();
                    }
                    for (const auto& submesh : mesh->GetSubMeshes()) {
                        Renderer::Submit(submesh.vertex_array, shader, transform);
                    }
                }
                Renderer::Disable(RenderSetting::FaceCulling);
                fbo->UnBind();
            }
        }
        {
            DE_PROFILE_SCOPE("Shader Update");
            auto skyboxes = m_Scene->m_Storage->View<SkyBoxComponent>();
            if (!skyboxes.Empty()) {
                skybox = *skyboxes.begin();
            }
            UpdateShaders(camera, skybox, lightCamera);
        }
        {
            DE_PROFILE_SCOPE("Mesh Rendering");
            m_FrameBuffer->Bind();
            m_Lights->Bind(LIGHT_UB_ID);
            if (settings.use_directional_shadow_map) {
                m_ShadowMap->GetDepthAttachment()->Bind(12);
            }
            if (settings.use_point_shadows) {
                int idx = 0;
                for (auto entity : m_Scene->m_Storage->View<LightSource>()) {
                    if (entity.Get<LightSource>().type == LightSourceType::Point) {
                        auto id = entity.Get<IDComponent>().Get();
                        m_PointShadowCubemaps[id]->Bind(15 + idx);
                        idx++;
                    }
                }
            }
            Renderer::SetViewport(m_FrameBuffer->GetWidth(), m_FrameBuffer->GetHeight());
            Renderer::Clear();
            if (skybox.Valid()) {
                Mat4 transform = Mat4(1.0);
                if (skybox.Has<TransformComponent>()) {
                    transform = skybox.Get<TransformComponent>().GetRotation();
                }
                static U32 cnt = 0;
                Renderer::Submit(skybox.Get<SkyBoxComponent>()->GetMap(), camera, transform);
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
                if (settings.use_directional_shadow_map) {
                    shader->SetInt("u_UseShadowMap", 1);
                    shader->SetInt("u_ShadowMap", 12);
                }
                if (settings.use_point_shadows) {
                    shader->SetInt("u_UsePointShadowMap", 1);
                    shader->SetFloat("u_ShadowFarPlane", Console::GetFloat("r_psm_far"));
                    int idx = 0;
                    for (auto entity : m_Scene->m_Storage->View<LightSource>()) {
                        if (entity.Get<LightSource>().type == LightSourceType::Point) {
                            shader->SetInt(StrCat("u_PointShadowMap[", idx, "]"), 15 + idx);
                            idx++;
                        }
                    }
                } else {
                    shader->SetInt("u_UsePointShadowMap", 0);
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

    void SceneRenderer::UpdateShaders(const FPSCamera& camera, Entity skybox, const FPSCamera& lightCamera) {
        DE_PROFILE_SCOPE("UpdateShaders");

        int cnt_light_sources = 0;
        int cnt_shadowmaps    = 0;
        for (auto enitity : m_Scene->m_Storage->View<LightSource>()) {
            auto& light_source                           = enitity.Get<LightSource>();
            auto  id                                     = enitity.Get<IDComponent>().Get();
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
            if (settings.use_directional_shadow_map) {
                shader->SetMat4("u_DirectionalShadowMapCamera.view", lightCamera.GetViewMatrix());
                shader->SetMat4("u_DirectionalShadowMapCamera.projection", lightCamera.GetOrthoProjectionMatrix());
                shader->SetInt("u_DirectionalShadowMap", 1);
            }
        }
    }

    Ref<FrameBuffer> SceneRenderer::GetFrameBuffer() {
        return m_FrameBuffer;
    }

    Ref<FrameBuffer> SceneRenderer::GetShadowMap() {
        return m_ShadowMap;
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
    void SceneRenderer::initializeShadowMap() {
        m_ShadowMap =
            FrameBuffer::Create({static_cast<U32>(Console::GetInt("r_shadowmap_width")), static_cast<U32>(Console::GetInt("r_shadowmap_height"))});
        m_ShadowMap->SetDepthAttachment(Texture::Format::F32);
        Console::OnCommand("r_shadowmap_resize", [&]() {
            Logger::Log(LogMessageType::Info, "ShadowMap resize");
            m_ShadowMap->Resize(static_cast<U32>(Console::GetInt("r_shadowmap_width")), static_cast<U32>(Console::GetInt("r_shadowmap_height")));
        });
    }
}  // namespace DE