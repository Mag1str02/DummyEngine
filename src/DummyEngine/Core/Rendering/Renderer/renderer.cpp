#include "DummyEngine/Core/Rendering/Renderer/renderer.h"
#include "DummyEngine/Core/Rendering/RendererOpenGL/gl_render_api.h"

namespace DE
{

    Scope<RenderAPI> Renderer::_render_api = nullptr;

    void Renderer::Init(API api)
    {
        switch (api)
        {
            case API::OpenGL: _render_api = CreateScope<GLRenderAPI>(); break;

            case API::Vulkan: _render_api = nullptr; break;
            case API::None: _render_api = nullptr; break;
        }
    }
    void Renderer::Load(const Window* window)
    {
        _render_api->Load(window);
        _render_api->SetDefaultState();
    }
    void Renderer::OnWindowResize(uint32_t width, uint32_t height)
    {
        _render_api->SetViewport(0, 0, width, height);
    }

    void Renderer::Clear()
    {
        _render_api->Clear();
    }

    void Renderer::Submit(Ref<Shader> shader, const Ref<VertexArray>& vertex_array, const Mat4& trasformation)
    {
        shader->Bind();
        vertex_array->Bind();
        glDrawElements(GL_TRIANGLES, vertex_array->GetIndexBuffer()->IndicesAmount(), GL_UNSIGNED_INT, 0);
    }
    void Renderer::Submit(Ref<Shader> shader, const RenderMesh& mesh, const Mat4& trasformation)
    {
        shader->Bind();
        shader->SetMaterial("u_Material", mesh.material);
        mesh.vertex_array->Bind();
        _render_api->DrawIndexed(mesh.vertex_array);
    }
    void Renderer::Submit(Ref<Shader> shader, const RenderModel& model, const Mat4& trasformation)
    {
        shader->Bind();
        for (const auto& mesh : model.meshes)
        {
            shader->SetMaterial("u_Material", mesh.material);
            mesh.vertex_array->Bind();
            _render_api->DrawIndexed(mesh.vertex_array);
        }
    }

    void Renderer::Enable(RenderSetting setting)
    {
        _render_api->Enable(setting);
    }
    void Renderer::Disable(RenderSetting setting)
    {
        _render_api->Disable(setting);
    }

    void Renderer::SetClearColor(Vec4 color)
    {
        _render_api->SetClearColor(color);
    }
    void Renderer::SetClearColor(float r, float g, float b, float a)
    {
        _render_api->SetClearColor(Vec4(r, g, b, a));
    }

    API Renderer::CurrentAPI()
    {
        return _render_api->GetAPI();
    }

}  // namespace DE
