#pragma once

#include "DummyEngine/Core/ECS/ECS.h"
#include "DummyEngine/Core/Objects/Cameras/FPSCamera.h"
#include "DummyEngine/Core/Scene/Scene.h"

namespace DummyEngine {

    class Shader;
    class UniformBuffer;
    class FrameBuffer;
    class CubeMap;

    class SceneRenderer {
        LOG_AUTHOR(SceneRenderer)
    public:
        explicit SceneRenderer(Scene* scene);

        void             Render(Entity camera);
        void             OnViewPortResize(U32 x, U32 y);
        Ref<FrameBuffer> GetFrameBuffer();
        Ref<FrameBuffer> GetShadowMap();

        void InitializeShadowMap();
        void RequestShader(UUID shader_id);

    public:
        struct GraphicsSettings {
            bool  Bloom              = false;
            float BloomThreshold     = 1;
            float BloomSoftThreshold = 0;
            float BloomRadius        = 0.005;
            float BloomStrength      = 0.04;
            U32   BloomDepth         = 5;

            bool  GammaToneMapping = false;
            float Exposure         = 1;
            float Gamma            = 1;

            bool UseDirectionalShadowMap = false;
            bool UsePointShadows         = false;
        };

    public:
        GraphicsSettings Settings;

    private:
        void UpdateShaders(const FPSCamera& camera, Entity skybox, const FPSCamera& light_camera);

    private:
        Scene*                                     scene_;
        Ref<UniformBuffer>                         lights_;
        Ref<FrameBuffer>                           frame_buffer_;
        Ref<FrameBuffer>                           shadow_map_;
        std::unordered_map<UUID, Ref<FrameBuffer>> point_shadow_map_;
        std::unordered_map<UUID, Ref<CubeMap>>     point_shadow_cubemaps_;
        std::unordered_map<UUID, Ref<Shader>>      shaders_;
    };

}  // namespace DummyEngine