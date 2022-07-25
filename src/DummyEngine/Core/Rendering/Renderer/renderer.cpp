#include "Core/Rendering/Renderer/Renderer.h"
#include "Core/Rendering/RendererOpenGL/GLRenderAPI.h"

namespace DE
{

    Scope<RenderAPI> Renderer::m_RenderAPI = nullptr;

    void Renderer::Init(API api)
    {
        switch (api)
        {
            case API::OpenGL: m_RenderAPI = CreateScope<GLRenderAPI>(); break;
            case API::Vulkan: {
                DE_ASSERT(false, "Attempt to init Renderer with VulkanAPI which is currently unsupported.");
                m_RenderAPI = nullptr;
                break;
            }
            case API::None: {
                DE_ASSERT(false, "Attempt to init Renderer without RenderingAPI.");
                m_RenderAPI = nullptr;
                break;
            }
        }
    }
    void Renderer::Load(const Window* window)
    {
        m_RenderAPI->Load(window);
        m_RenderAPI->SetDefaultState();
    }
    void Renderer::OnWindowResize(uint32_t width, uint32_t height)
    {
        m_RenderAPI->SetViewport(0, 0, width, height);
    }

    void Renderer::Clear()
    {
        m_RenderAPI->Clear();
    }

    void Renderer::Submit(Ref<Shader> shader, const Ref<VertexArray>& vertex_array, const Mat4& trasform)
    {
        shader->Bind();
        shader->SetMat4("u_Transform", trasform);
        vertex_array->Bind();
        glDrawElements(GL_TRIANGLES, vertex_array->GetIndexBuffer()->IndicesAmount(), GL_UNSIGNED_INT, 0);
    }
    void Renderer::Submit(Ref<Shader> shader, const RenderMesh& mesh, const Mat4& trasform)
    {
        shader->Bind();
        shader->SetMaterial("u_Material", mesh.material);
        shader->SetMat4("u_Transform", trasform);
        mesh.vertex_array->Bind();
        m_RenderAPI->DrawIndexed(mesh.vertex_array);
    }
    void Renderer::Submit(Ref<Shader> shader, const RenderModel& model, const Mat4& trasform)
    {
        shader->Bind();
        shader->SetMat4("u_Transform", trasform);
        for (const auto& mesh : model.meshes)
        {
            shader->SetMaterial("u_Material", mesh.material);
            mesh.vertex_array->Bind();
            m_RenderAPI->DrawIndexed(mesh.vertex_array);
        }
    }

    void Renderer::Enable(RenderSetting setting)
    {
        m_RenderAPI->Enable(setting);
    }
    void Renderer::Disable(RenderSetting setting)
    {
        m_RenderAPI->Disable(setting);
    }

    void Renderer::SetClearColor(Vec4 color)
    {
        m_RenderAPI->SetClearColor(color);
    }
    void Renderer::SetClearColor(float r, float g, float b, float a)
    {
        m_RenderAPI->SetClearColor(Vec4(r, g, b, a));
    }

    API Renderer::CurrentAPI()
    {
        return m_RenderAPI->GetAPI();
    }

}  // namespace DE
