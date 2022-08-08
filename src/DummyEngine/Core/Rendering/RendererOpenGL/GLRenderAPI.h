#pragma once

#include "Core/Rendering/Renderer/RenderAPI.h"

namespace DE
{
    class GLRenderAPI : public RenderAPI
    {
    public:
        GLRenderAPI() = default;
        virtual ~GLRenderAPI() = default;

        virtual void SetDefaultState() override;

        virtual bool Load(const Window* window) override;
        virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

        virtual void Clear() override;
        virtual void DrawIndexed(const Ref<VertexArray>& vertex_array) override;
        virtual void DrawInstanced(const Ref<VertexArray>& vertex_array, uint32_t instance_count) override;

        virtual void SetLineWidth(float width) override;
        virtual void SetClearColor(const Vec4& color) override;
        virtual void Enable(RenderSetting setting) override;
        virtual void Disable(RenderSetting setting) override;

        virtual API GetAPI() override;
    };
}  // namespace DE