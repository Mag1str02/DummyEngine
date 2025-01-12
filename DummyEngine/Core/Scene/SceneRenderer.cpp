#include "SceneRenderer.h"

#include "DummyEngine/Core/Console/Console.hpp"
#include "DummyEngine/Core/Objects/LightSources/LightSource.h"
#include "DummyEngine/Core/Rendering/Renderer/FrameBuffer.h"
#include "DummyEngine/Core/Rendering/Renderer/Renderer.h"
#include "DummyEngine/Core/Rendering/Renderer/UniformBuffer.h"
#include "DummyEngine/Core/ResourceManaging/ResourceManager.h"
#include "DummyEngine/Core/Scene/Components.h"
#include "DummyEngine/Core/Scripting/ScriptEngine.h"
#include "DummyEngine/Utils/Debug/Profiler.h"

namespace DummyEngine {

    constexpr U32 kMaxLightsInScene     = 1000;
    constexpr U32 kLightUniformBufferID = 1;

    SceneRenderer::SceneRenderer(Scene* scene) : scene_(scene) {
        lights_ = UniformBuffer::Create(
            {
                BufferElementType::Float3,
                BufferElementType::Float3,
                BufferElementType::Float3,
                BufferElementType::Float3,
                BufferElementType::Float3,
                BufferElementType::Float3,
                BufferElementType::Float3,
            },
            kMaxLightsInScene);
        frame_buffer_ = FrameBuffer::Create({1920, 1080});
        frame_buffer_->AddColorAttachment(Texture::Format::F32, Texture::Channels::RGBA);
        frame_buffer_->SetDepthAttachment(Texture::Format::F32);

        if (Settings.UseDirectionalShadowMap) {
            InitializeShadowMap();
        }
    }
    void SceneRenderer::OnViewPortResize(U32 x, U32 y) {
        frame_buffer_->Resize(x, y);
    }
    void SceneRenderer::Render(Entity cam) {
        DE_PROFILE_SCOPE("Scene Render");
        Entity skybox;

        auto&     camera = cam.Get<FPSCamera>();
        FPSCamera light_camera(Vec3{0, 100, 0}, Vec3{0, -1, 0});
        if (Settings.UseDirectionalShadowMap) {
            DE_PROFILE_SCOPE("Directional ShadowMap Rendering");
            if (!shadow_map_) {
                InitializeShadowMap();
            }
            shadow_map_->Bind();
            Renderer::Enable(RenderSetting::FaceCulling);
            Renderer::SetViewport(shadow_map_->GetWidth(), shadow_map_->GetHeight());
            Renderer::Clear();
            auto shader = Renderer::GetShader(Renderer::Shaders::DirectionalShadowMap);
            shader->Bind();

            for (auto entity : scene_->storage_->View<LightSource, TransformComponent>()) {
                if (entity.Get<LightSource>().Type == LightSourceType::Direction) {
                    light_camera.SetPos(entity.Get<TransformComponent>().Translation);
                    light_camera.SetDir(entity.Get<LightSource>().Direction);
                }
            }
            shader->SetMat4("u_Camera.view", light_camera.GetViewMatrix());
            shader->SetMat4("u_Camera.projection", light_camera.GetOrthoProjectionMatrix());
            for (auto e : scene_->View<RenderMeshComponent>()) {
                auto mesh = e.Get<RenderMeshComponent>().Mesh;
                int  res  = (mesh->Animator ? 1 : 0);
                shader->SetInt("u_Animated", res);
                if (mesh->Animator) {
                    mesh->Animator->SetMatricies(shader);
                }
                Mat4 transform(1.0f);
                if (e.Has<TransformComponent>()) {
                    transform = e.Get<TransformComponent>().GetTransform();
                }
                for (const auto& submesh : mesh->GetSubMeshes()) {
                    Renderer::Submit(submesh.VertexArray, shader, transform);
                }
            }
            Renderer::Disable(RenderSetting::FaceCulling);
            shadow_map_->UnBind();
        }
        if (Settings.UsePointShadows) {
            DE_PROFILE_SCOPE("PointShadowMap Rendering");
            auto shader = Renderer::GetShader(Renderer::Shaders::OmnidirectionalShadowMap);
            shader->Bind();
            U32       size        = Console::GetInt("r_psm_size");
            glm::mat4 shadow_proj = glm::perspective(glm::radians(90.f), 1.f, Console::GetFloat("r_psm_near"), Console::GetFloat("r_psm_far"));
            for (auto entity : scene_->storage_->View<LightSource>()) {
                auto  id           = entity.Get<IDComponent>().Get();
                auto& light_source = entity.Get<LightSource>();
                if (light_source.Type != LightSourceType::Point) {
                    continue;
                }
                Ref<FrameBuffer> fbo;
                if (point_shadow_cubemaps_.find(id) == point_shadow_cubemaps_.end()) {
                    auto map = CubeMap::Create(size, Texture::Format::F32, Texture::Channels::Depth, false, true);
                    fbo      = FrameBuffer::Create({size, size});
                    fbo->SetDepthAttachment(map);
                    point_shadow_cubemaps_[id] = map;
                    point_shadow_map_[id]      = fbo;
                } else {
                    fbo = point_shadow_map_[id];
                }
                fbo->Bind();
                Renderer::Enable(RenderSetting::FaceCulling);
                Renderer::SetViewport(size, size);
                Renderer::Clear();
                auto                   pos = light_source.Position;
                std::vector<glm::mat4> shadow_matrices(6);
                shadow_matrices[0] = shadow_proj * glm::lookAt(pos, pos + glm::vec3(1., 0., 0.), glm::vec3(0., -1., 0.));
                shadow_matrices[1] = shadow_proj * glm::lookAt(pos, pos + glm::vec3(-1., 0., 0.), glm::vec3(0., -1., 0.));
                shadow_matrices[2] = shadow_proj * glm::lookAt(pos, pos + glm::vec3(0., 1., 0.), glm::vec3(0., 0., 1.));
                shadow_matrices[3] = shadow_proj * glm::lookAt(pos, pos + glm::vec3(0., -1., 0.), glm::vec3(0., 0., -1.));
                shadow_matrices[4] = shadow_proj * glm::lookAt(pos, pos + glm::vec3(0., 0., 1.), glm::vec3(0., -1., 0.));
                shadow_matrices[5] = shadow_proj * glm::lookAt(pos, pos + glm::vec3(0., 0., -1.), glm::vec3(0., -1., 0.));
                for (size_t i = 0; i < 6; i++) {
                    shader->SetMat4(std::format("shadowMatrices[{}]", i), shadow_matrices[i]);
                }
                shader->SetFloat("far_plane", Console::GetFloat("r_psm_far"));
                shader->SetFloat3("lightPos", pos);
                for (auto e : scene_->View<RenderMeshComponent>()) {
                    auto mesh = e.Get<RenderMeshComponent>().Mesh;
                    int  res  = (mesh->Animator ? 1 : 0);
                    shader->SetInt("u_Animated", res);
                    if (mesh->Animator) {
                        mesh->Animator->SetMatricies(shader);
                    }
                    Mat4 transform(1.0f);
                    if (e.Has<TransformComponent>()) {
                        transform = e.Get<TransformComponent>().GetTransform();
                    }
                    for (const auto& submesh : mesh->GetSubMeshes()) {
                        Renderer::Submit(submesh.VertexArray, shader, transform);
                    }
                }
                Renderer::Disable(RenderSetting::FaceCulling);
                fbo->UnBind();
            }
        }
        {
            DE_PROFILE_SCOPE("Shader Update");
            auto skyboxes = scene_->storage_->View<SkyBoxComponent>();
            if (!skyboxes.Empty()) {
                skybox = *skyboxes.begin();
            }
            UpdateShaders(camera, skybox, light_camera);
        }
        {
            DE_PROFILE_SCOPE("Mesh Rendering");
            frame_buffer_->Bind();
            lights_->Bind(kLightUniformBufferID);
            if (Settings.UseDirectionalShadowMap) {
                shadow_map_->GetDepthAttachment()->Bind(12);
            }
            if (Settings.UsePointShadows) {
                int idx = 0;
                for (auto entity : scene_->storage_->View<LightSource>()) {
                    if (entity.Get<LightSource>().Type == LightSourceType::Point) {
                        auto id = entity.Get<IDComponent>().Get();
                        point_shadow_cubemaps_[id]->Bind(15 + idx);
                        idx++;
                    }
                }
            }
            Renderer::SetViewport(frame_buffer_->GetWidth(), frame_buffer_->GetHeight());
            Renderer::Clear();
            if (skybox.Valid()) {
                Mat4 transform = Mat4(1.0);
                if (skybox.Has<TransformComponent>()) {
                    transform = skybox.Get<TransformComponent>().GetRotation();
                }
                Renderer::Submit(skybox.Get<SkyBoxComponent>()->GetMap(), camera, transform);
            }
            for (auto e : scene_->View<ScriptComponent>()) {
                auto& component = e.Get<ScriptComponent>();
                if (component.Valid()) {
                    component->OnRender();
                }
            }
            for (auto e : scene_->View<RenderMeshComponent, ShaderComponent>()) {
                auto shader = e.Get<ShaderComponent>().Shader;
                auto mesh   = e.Get<RenderMeshComponent>().Mesh;
                int  res    = (mesh->Animator ? 1 : 0);
                shader->Bind();
                shader->SetInt("u_Animated", res);
                if (mesh->Animator) {
                    mesh->Animator->SetMatricies(shader);
                }
                Mat4 transform(1.0f);
                if (e.Has<TransformComponent>()) {
                    transform = e.Get<TransformComponent>().GetTransform();
                }
                if (Settings.UseDirectionalShadowMap) {
                    shader->SetInt("u_UseShadowMap", 1);
                    shader->SetInt("u_ShadowMap", 12);
                }
                if (Settings.UsePointShadows) {
                    shader->SetInt("u_UsePointShadowMap", 1);
                    shader->SetFloat("u_ShadowFarPlane", Console::GetFloat("r_psm_far"));
                    int idx = 0;
                    for (auto entity : scene_->storage_->View<LightSource>()) {
                        if (entity.Get<LightSource>().Type == LightSourceType::Point) {
                            shader->SetInt(std::format("u_PointShadowMap[{}]", idx), 15 + idx);
                            idx++;
                        }
                    }
                } else {
                    shader->SetInt("u_UsePointShadowMap", 0);
                }
                Renderer::Submit(mesh, shader, transform);
            }
            frame_buffer_->UnBind();
        }
        {
            DE_PROFILE_SCOPE("Post-Processing");
            if (Settings.Bloom) {
                DE_PROFILE_SCOPE("Bloom");
                Renderer::Bloom(frame_buffer_->GetColorAttachment(0),
                                Settings.BloomThreshold,
                                Settings.BloomSoftThreshold,
                                Settings.BloomRadius,
                                Settings.BloomDepth,
                                Settings.BloomStrength);
            }
            if (Settings.GammaToneMapping) {
                DE_PROFILE_SCOPE("Gamma & HDR");
                Renderer::GammeHDRCorrecion(frame_buffer_->GetColorAttachment(0), Settings.Exposure, Settings.Gamma);
            }
        }
    }

    void SceneRenderer::UpdateShaders(const FPSCamera& camera, Entity skybox, const FPSCamera& light_camera) {
        DE_PROFILE_SCOPE("UpdateShaders");

        int cnt_light_sources = 0;
        for (auto enitity : scene_->storage_->View<LightSource>()) {
            auto& light_source                          = enitity.Get<LightSource>();
            lights_->At(cnt_light_sources).Get<Vec3>(0) = light_source.Ambient;
            lights_->At(cnt_light_sources).Get<Vec3>(1) = light_source.Diffuse;
            lights_->At(cnt_light_sources).Get<Vec3>(2) = light_source.Specular;
            lights_->At(cnt_light_sources).Get<Vec3>(3) = light_source.Direction;
            lights_->At(cnt_light_sources).Get<Vec3>(4) = light_source.Position;
            lights_->At(cnt_light_sources).Get<Vec3>(5) = light_source.CLQ;
            lights_->At(cnt_light_sources).Get<Vec3>(6) =
                Vec3(light_source.OuterConeCosinus, light_source.InnerConeCosinus, LightSourceTypeToId(light_source.Type));
            cnt_light_sources++;
        }
        lights_->PushData();
        for (auto [id, shader] : shaders_) {
            shader->Bind();
            if (skybox.Valid() && skybox.Has<SkyBoxComponent>()) {
                skybox.Get<SkyBoxComponent>()->ApplyIBL(shader);
                if (skybox.Has<TransformComponent>()) {
                    auto& transform   = skybox.Get<TransformComponent>();
                    Vec3  rotation    = -(transform.Rotation + transform.RotationOffet);
                    Mat4  mt_rotation = glm::toMat4(glm::quat(glm::radians(rotation)));
                    shader->SetMat4("u_EnvRotation", mt_rotation);
                }
            }
            shader->SetInt("u_LightAmount", cnt_light_sources);
            shader->SetMat4("u_Camera.view", camera.GetViewMatrix());
            shader->SetMat4("u_Camera.projection", camera.GetProjectionMatrix());
            if (Settings.UseDirectionalShadowMap) {
                shader->SetMat4("u_DirectionalShadowMapCamera.view", light_camera.GetViewMatrix());
                shader->SetMat4("u_DirectionalShadowMapCamera.projection", light_camera.GetOrthoProjectionMatrix());
                shader->SetInt("u_DirectionalShadowMap", 1);
            }
        }
    }

    Ref<FrameBuffer> SceneRenderer::GetFrameBuffer() {
        return frame_buffer_;
    }

    Ref<FrameBuffer> SceneRenderer::GetShadowMap() {
        return shadow_map_;
    }

    void SceneRenderer::RequestShader(UUID shader_id) {
        if (!shaders_.contains(shader_id)) {
            auto shader = ResourceManager::GetShader(shader_id);
            if (shader) {
                shaders_[shader_id] = shader.value();
                shaders_[shader_id]->SetUnifromBlock("ub_Lights", kLightUniformBufferID);
            } else {
                LOG_WARNING("Shader {} not found in ResourceManager", shader_id);
            }
        }
    }
    void SceneRenderer::InitializeShadowMap() {
        shadow_map_ =
            FrameBuffer::Create({static_cast<U32>(Console::GetInt("r_shadowmap_width")), static_cast<U32>(Console::GetInt("r_shadowmap_height"))});
        shadow_map_->SetDepthAttachment(Texture::Format::F32);
        Console::OnCommand("r_shadowmap_resize", [&]() {
            LOG_INFO("ShadowMap resize");
            shadow_map_->Resize(static_cast<U32>(Console::GetInt("r_shadowmap_width")), static_cast<U32>(Console::GetInt("r_shadowmap_height")));
            point_shadow_cubemaps_.clear();
            point_shadow_map_.clear();
        });
    }
}  // namespace DummyEngine