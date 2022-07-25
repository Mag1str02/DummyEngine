#pragma once

#include "DummyEngine/Addition/Base.h"
#include "DummyEngine/Core/Rendering/Renderer/VertexArray.h"
#include "DummyEngine/Core/Rendering/Renderer/Texture.h"
#include "DummyEngine/Core/Rendering/Renderer/RenderAPI.h"
#include "DummyEngine/Core/Rendering/Renderer/Shader.h"

namespace DE
{

    class Renderer
    {
    public:
        static void Init(API api);
        static void Load(const Window* window);
        static void OnWindowResize(uint32_t width, uint32_t height);

        static void Clear();
        static void Submit(Ref<Shader> shader, const Ref<VertexArray>& mesh, const Mat4& trasform = Mat4(1.0f));
        static void Submit(Ref<Shader> shader, const RenderMesh& mesh, const Mat4& trasform = Mat4(1.0f));
        static void Submit(Ref<Shader> shader, const RenderModel& mesh, const Mat4& trasform = Mat4(1.0f));

        static void Enable(RenderSetting setting);
        static void Disable(RenderSetting setting);
        static void SetClearColor(Vec4 color);
        static void SetClearColor(float r, float g, float b, float a);

        static API CurrentAPI();

    private:
        static Scope<RenderAPI> m_RenderAPI;
    };

}  // namespace DE
