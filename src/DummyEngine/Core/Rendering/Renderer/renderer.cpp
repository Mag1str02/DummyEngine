#include "Core/Rendering/Renderer/Renderer.h"
#include "Core/Rendering/RendererOpenGL/GLRenderAPI.h"
#include "ToolBox/Loaders/TextureLoader.h"

namespace DE
{
    Scope<FrameStatistics> Renderer::m_FrameStatistics = nullptr;
    Scope<RenderAPI> Renderer::m_RenderAPI = nullptr;
    Ref<Texture> Renderer::m_DefaultTexture = nullptr;

    void FrameStatistics::Reset()
    {
        m_DrawCallsAmount = 0;
    }

    void Renderer::Init(API api)
    {
        m_FrameStatistics = CreateScope<FrameStatistics>();
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
        GenDefaultTexture();
    }
    void Renderer::OnWindowResize(uint32_t width, uint32_t height)
    {
        m_RenderAPI->SetViewport(0, 0, width, height);
    }

    void Renderer::BeginFrame()
    {
        m_FrameStatistics->Reset();
    }
    void Renderer::EndFrame() {}

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
        ++m_FrameStatistics->m_DrawCallsAmount;
    }
    void Renderer::Submit(Ref<Shader> shader, const RenderMesh& mesh, const Mat4& trasform)
    {
        shader->Bind();
        shader->SetMaterial("u_Material", mesh.material);
        shader->SetMat4("u_Transform", trasform);
        mesh.vertex_array->Bind();
        m_RenderAPI->DrawIndexed(mesh.vertex_array);
        ++m_FrameStatistics->m_DrawCallsAmount;
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
            ++m_FrameStatistics->m_DrawCallsAmount;
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

    Ref<Texture> Renderer::GetDefaultTexture()
    {
        return m_DefaultTexture;
    }
    API Renderer::CurrentAPI()
    {
        return m_RenderAPI->GetAPI();
    }
    FrameStatistics Renderer::GetStatistics()
    {
        return *m_FrameStatistics;
    }

    //TODO: Think to move somewhere else...
    void Renderer::GenDefaultTexture()
    {
        Ref<TextureData> texture_data = CreateRef<TextureData>();

        texture_data->width = 1;
        texture_data->height = 1;
        texture_data->format = TextureFormat::RGBA;
        texture_data->data = CreateRef<unsigned char*>((unsigned char*)malloc(sizeof(unsigned char) * 4));

        (*texture_data->data)[0] = 255;
        (*texture_data->data)[1] = 255;
        (*texture_data->data)[2] = 255;
        (*texture_data->data)[3] = 255;

        m_DefaultTexture = Texture::Create(*texture_data);
    }

}  // namespace DE
