#pragma once

#include "DummyEngine/Core/ECS/ECS.h"
#include "DummyEngine/Core/Rendering/Renderer/FrameBuffer.h"
#include "DummyEngine/Core/Rendering/Renderer/Shader.h"
#include "DummyEngine/Core/Rendering/Renderer/UniformBuffer.h"
#include "DummyEngine/Core/Scene/Scene.h"
#include "DummyEngine/Utils/Base.h"

namespace DE {
    class SceneRenderer {
    public:
        struct Settings {
            bool  bloom                = false;
            float bloom_threshold      = 1;
            float bloom_soft_threshold = 0;
            float bloom_radius         = 0.005;
            float bloom_strength       = 0.04;
            U32   bloom_depth          = 5;

            bool  gamma_tone_mapping = false;
            float exposure           = 1;
            float gamma              = 1;

            bool use_directional_shadow_map = false;
            bool use_point_shadows = false;
        };
        Settings settings;
        SceneRenderer(Scene* scene);

        void             Render(Entity camera);
        void             OnViewPortResize(U32 x, U32 y);
        Ref<FrameBuffer> GetFrameBuffer();
        Ref<FrameBuffer> GetShadowMap();

        void initializeShadowMap();

        void RequestShader(UUID shader_id);

    private:
        void UpdateShaders(const FPSCamera& camera, Entity skybox, const FPSCamera& lightCamera);

        Scene*                                m_Scene;
        Ref<UniformBuffer>                    m_Lights;
        Ref<FrameBuffer>                      m_FrameBuffer;
        Ref<FrameBuffer>                      m_ShadowMap;
        std::unordered_map<UUID, Ref<FrameBuffer> > m_PointShadowFrameBuffers;
        std::unordered_map<UUID, Ref<CubeMap> > m_PointShadowCubemaps;
        std::unordered_map<UUID, Ref<Shader>> m_Shaders;
    };
}  // namespace DE