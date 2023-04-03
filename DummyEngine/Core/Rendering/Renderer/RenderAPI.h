#pragma once

#include "DummyEngine/Utils/Base.h"
#include "DummyEngine/Core/Rendering/Renderer/VertexArray.h"

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

        virtual void SetViewport(U32 x, U32 y, U32 width, U32 height) = 0;

        virtual void Clear()                                                                                                                             = 0;
        virtual void DrawArrays(const Ref<VertexArray>& vertex_array, RenderPrimitive primitive = RenderPrimitive::Triangle)                              = 0;
        virtual void DrawIndexed(const Ref<VertexArray>& vertex_array, RenderPrimitive primitive = RenderPrimitive::Triangle)                            = 0;
        virtual void DrawInstanced(const Ref<VertexArray>& vertex_array, U32 instance_count, RenderPrimitive primitive = RenderPrimitive::Triangle) = 0;

        virtual void SetLineWidth(float width)        = 0;
        virtual void SetClearColor(const Vec4& color) = 0;
        virtual void Enable(RenderSetting setting)    = 0;
        virtual void Disable(RenderSetting setting)   = 0;

        virtual API GetAPI() = 0;
    };
}  // namespace DE