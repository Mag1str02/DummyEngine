#pragma once

#include "DummyEngine/Core/Objects/Cameras/FPSCamera.h"
#include "DummyEngine/Core/Rendering/Renderer/CubeMap.h"
#include "DummyEngine/Core/Rendering/Renderer/FrameBuffer.h"
#include "DummyEngine/Core/Rendering/Renderer/RenderAPI.h"
#include "DummyEngine/Core/Rendering/Renderer/RenderStructs.h"
#include "DummyEngine/Core/Rendering/Renderer/Shader.h"
#include "DummyEngine/Core/Rendering/Renderer/Texture.h"
#include "DummyEngine/Core/Rendering/Renderer/VertexArray.h"
#include "DummyEngine/Utils/Base.h"

namespace DE {

    class Renderer : public Singleton<Renderer> {
        SINGLETON(Renderer)
    public:
        enum class Shaders {
            None = 0,
            Skybox,
            Convolution,
            PreFileterConvolution,
            EquirectangularToCubeMap,
            BRDFConvolution,
            BrightnessFilter,
            TexturedQuad,
            GaussianBlur,
            Bloom,
            GammaHDR,

            Last,
        };
        enum class Textures {
            None = 0,
            White,
            Normal,
            BRDF,

            Last,
        };
        enum class VertexArrays {
            None = 0,
            ScreenQuad,
            Cube,

            Last,
        };
        struct Statistics {
            U32 m_DrawCallsAmount;
            U32 m_DrawnInstances;

            void Reset();
        };

        S_METHOD_DEF(Unit, SetViewport, (U32 width, U32 height));
        S_METHOD_DEF(Unit, BeginFrame, ());
        S_METHOD_DEF(Unit, EndFrame, ());
        S_METHOD_DEF(Unit, Clear, ());

        S_METHOD_DEF(Unit, Submit, (Ref<VertexArray> vertex_array, Ref<Shader> shader, const Mat4& transform = Mat4(1.0f)));
        S_METHOD_DEF(Unit, Submit, (Ref<RenderMesh> mesh, Ref<Shader> shader, const Mat4& transform = Mat4(1.0f)));
        S_METHOD_DEF(Unit, Submit, (Ref<CubeMap> cube_map, const FPSCamera& camera, const Mat4& transform = Mat4(1.0f)));

        S_METHOD_DEF(Unit, GammeHDRCorrecion, (Ref<FrameBuffer> buffer, float exposure, float gamma));

        S_METHOD_DEF(Unit, Enable, (RenderSetting setting));
        S_METHOD_DEF(Unit, Disable, (RenderSetting setting));
        S_METHOD_DEF(Unit, SetClearColor, (Vec4 color));
        S_METHOD_DEF(Unit, SetClearColor, (float r, float g, float b, float a));
        S_METHOD_DEF(Unit, SetDefaultFrameBuffer, ());

        S_METHOD_DEF(Ref<Shader>, GetShader, (Shaders shader));
        S_METHOD_DEF(Ref<Texture>, GetTexture, (Textures texture));
        S_METHOD_DEF(Ref<VertexArray>, GetVertexArray, (VertexArrays vao));

        S_METHOD_DEF(API, CurrentAPI, ());
        S_METHOD_DEF(Statistics, GetStatistics, ());
        S_METHOD_DEF(RenderAPI&, GetRenderAPI, ());

    private:
        void GenResources();

        struct Resources {
            Ref<Texture> white;
            Ref<Texture> normal;
            Ref<Texture> brdf;

            Ref<VertexArray> screen_quad;
            Ref<VertexArray> cube;

            Ref<Shader> equirectangular_to_cubemap;
            Ref<Shader> skybox;
            Ref<Shader> convolution;
            Ref<Shader> pre_filter_convolution;
            Ref<Shader> brdf_convolution;
            Ref<Shader> brightness_filter;
            Ref<Shader> gaussian_blur;
            Ref<Shader> copy_texture;
            Ref<Shader> bloom;
            Ref<Shader> gamma_hdr;
        };

        Scope<RenderAPI> m_RenderAPI;
        Statistics       m_FrameStatistics;
        Resources        m_Resources;
    };

}  // namespace DE
