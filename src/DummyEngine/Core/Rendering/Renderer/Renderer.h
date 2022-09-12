#pragma once

#include "Addition/Base.h"
#include "Core/Rendering/Renderer/VertexArray.h"
#include "Core/Rendering/Renderer/Texture.h"
#include "Core/Rendering/Renderer/RenderAPI.h"
#include "Core/Rendering/Renderer/Shader.h"
#include "Core/Rendering/Renderer/CubeMap.h"

namespace DE
{
    struct FrameStatistics
    {
        uint32_t m_DrawCallsAmount;
        uint32_t m_DrawnInstances;

        void Reset();
    };

    class Renderer
    {
    public:
        static void Init(API api);
        static void OnWindowResize(uint32_t width, uint32_t height);

        static void BeginFrame();
        static void EndFrame();

        static void Clear();

        static void Submit(const Ref<VertexArray>& mesh, Ref<Shader> shader, const Mat4& trasform = Mat4(1.0f));
        static void Submit(Ref<RenderMesh> mesh, Ref<Shader> shader, const Mat4& trasform = Mat4(1.0f));
        static void Submit(Ref<CubeMap> cube_map, Ref<Shader> shader, const Mat4& trasform = Mat4(1.0f));

        static void Enable(RenderSetting setting);
        static void Disable(RenderSetting setting);
        static void SetClearColor(Vec4 color);
        static void SetClearColor(float r, float g, float b, float a);

        // TMP
        static Ref<Texture>     GetDefaultTexture();
        static Ref<VertexArray> GetFullScreenQuad();
        static Ref<VertexArray> GetCube();

        static API             CurrentAPI();
        static FrameStatistics GetStatistics();
        static RenderAPI& GetRenderAPI();

    private:
        static void GenDefaultTexture();
        static void GenFullScreenQuad();
        static void GenCube();

        static Scope<FrameStatistics> m_FrameStatistics;
        static Scope<RenderAPI>       m_RenderAPI;
        static Ref<Texture>           m_DefaultTexture;
        static Ref<VertexArray>       m_FullScreenQuad;
        static Ref<VertexArray>       m_Cube;
    };

}  // namespace DE
