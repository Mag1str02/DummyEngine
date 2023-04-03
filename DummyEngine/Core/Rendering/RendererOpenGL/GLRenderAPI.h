#pragma once

#include "DummyEngine/Core/Rendering/Renderer/RenderAPI.h"

namespace DE
{
    class GLRenderAPI : public RenderAPI
    {
    public:
        GLRenderAPI()          = default;
        virtual ~GLRenderAPI() = default;

        virtual void SetDefaultState() override;

        virtual void SetViewport(U32 x, U32 y, U32 width, U32 height) override;

        virtual void Clear() override;
        virtual void DrawArrays(const Ref<VertexArray>& vertex_array, RenderPrimitive primitive = RenderPrimitive::Triangle) override;
        virtual void DrawIndexed(const Ref<VertexArray>& vertex_array, RenderPrimitive primitive = RenderPrimitive::Triangle) override;
        virtual void DrawInstanced(const Ref<VertexArray>& vertex_array, U32 instance_count, RenderPrimitive primitive = RenderPrimitive::Triangle) override;

        virtual void SetLineWidth(float width) override;
        virtual void SetClearColor(const Vec4& color) override;
        virtual void Enable(RenderSetting setting) override;
        virtual void Disable(RenderSetting setting) override;

        virtual API GetAPI() override;
    };
}  // namespace DE