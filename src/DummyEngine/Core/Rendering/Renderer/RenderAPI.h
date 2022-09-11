#pragma once

#include "Addition/Base.h"
#include "Core/Rendering/Renderer/VertexArray.h"

namespace DE
{
    enum class RenderSetting
    {
        None = 0,
        DepthTest,
        MultiSampling,
        FaceCulling,
        DepthMask
    };
    enum class RenderPrimitive
    {
        None = 0,
        Triangle,
        Line,
        Point
    };
    class RenderAPI
    {
    public:
        virtual ~RenderAPI()           = default;
        virtual void SetDefaultState() = 0;

        virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;

        virtual void Clear()                                                                                                                             = 0;
        virtual void DrawArrays(const Ref<VertexArray>& vertex_array, RenderPrimitive primitive = RenderPrimitive::Triangle)                              = 0;
        virtual void DrawIndexed(const Ref<VertexArray>& vertex_array, RenderPrimitive primitive = RenderPrimitive::Triangle)                            = 0;
        virtual void DrawInstanced(const Ref<VertexArray>& vertex_array, uint32_t instance_count, RenderPrimitive primitive = RenderPrimitive::Triangle) = 0;

        virtual void SetLineWidth(float width)        = 0;
        virtual void SetClearColor(const Vec4& color) = 0;
        virtual void Enable(RenderSetting setting)    = 0;
        virtual void Disable(RenderSetting setting)   = 0;

        virtual API GetAPI() = 0;
    };
}  // namespace DE