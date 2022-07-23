#pragma once

#include "DummyEngine/Addition/base.h"
#include "DummyEngine/Core/Rendering/Renderer/vertex_array.h"
#include "DummyEngine/Core/Rendering/Renderer/texture.h"
#include "DummyEngine/Core/Rendering/Renderer/render_api.h"
#include "DummyEngine/Core/Rendering/Renderer/shader.h"

namespace DE
{

    class Renderer
    {
    public:
        static void Init(API api);
        static void Load(const Window* window);
        static void OnWindowResize(uint32_t width, uint32_t height);

        static void Clear();
        static void Submit(Ref<Shader> shader, const Ref<VertexArray>& mesh, const Mat4& trasgormation = Mat4(1.0f));
        static void Submit(Ref<Shader> shader, const RenderMesh& mesh, const Mat4& trasgormation = Mat4(1.0f));
        static void Submit(Ref<Shader> shader, const RenderModel& mesh, const Mat4& trasgormation = Mat4(1.0f));

        static void Enable(RenderSetting setting);
        static void Disable(RenderSetting setting);
        static void SetClearColor(Vec4 color);
        static void SetClearColor(float r, float g, float b, float a);

        static API CurrentAPI();

    private:
        static Scope<RenderAPI> _render_api;
    };

}  // namespace DE
