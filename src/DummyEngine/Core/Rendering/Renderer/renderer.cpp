#include "Core/Rendering/Renderer/Renderer.h"
#include "Core/Rendering/RendererOpenGL/GLRenderAPI.h"
#include "ToolBox/Loaders/TextureLoader.h"

namespace DE
{
    Scope<FrameStatistics> Renderer::m_FrameStatistics = nullptr;
    Scope<RenderAPI> Renderer::m_RenderAPI = nullptr;
    Ref<Texture> Renderer::m_DefaultTexture = nullptr;
    Ref<VertexArray> Renderer::m_FullScreenQuad = nullptr;

    void FrameStatistics::Reset()
    {
        m_DrawCallsAmount = 0;
        m_DrawnInstances = 0;
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
        GenFullScreenQuad();
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
    void Renderer::Submit(const Ref<VertexArray>& vertex_array, Ref<Shader> shader, const Mat4& trasform)
    {
        shader->Bind();
        shader->SetMat4("u_Transform", trasform);
        vertex_array->Bind();
        glDrawElements(GL_TRIANGLES, vertex_array->GetIndexBuffer()->IndicesAmount(), GL_UNSIGNED_INT, 0);
        ++m_FrameStatistics->m_DrawCallsAmount;
    }
    void Renderer::Submit(Ref<RenderMesh> mesh, Ref<Shader> shader, const Mat4& trasform)
    {
        shader->Bind();
        if (mesh->m_InstanceBuffer)
        {
            shader->SetInt("u_Instanced", 1);
            for (const auto& sub_mesh : mesh->m_SubMeshes)
            {
                shader->SetMaterial("u_Material", sub_mesh.material);
                sub_mesh.vertex_array->Bind();
                m_RenderAPI->DrawInstanced(sub_mesh.vertex_array, mesh->m_Instances.size());
                ++m_FrameStatistics->m_DrawCallsAmount;
                m_FrameStatistics->m_DrawnInstances += mesh->m_Instances.size();
            }
        }
        else
        {
            shader->SetInt("u_Instanced", 0);
            shader->SetMat4("u_Transform", trasform);
            for (const auto& sub_mesh : mesh->m_SubMeshes)
            {
                shader->SetMaterial("u_Material", sub_mesh.material);
                sub_mesh.vertex_array->Bind();
                m_RenderAPI->DrawInstanced(sub_mesh.vertex_array, 1);
                ++m_FrameStatistics->m_DrawCallsAmount;
                ++m_FrameStatistics->m_DrawnInstances;
            }
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
    Ref<VertexArray> Renderer::GetFullScreenQuad()
    {
        return m_FullScreenQuad;
    }
    API Renderer::CurrentAPI()
    {
        return m_RenderAPI->GetAPI();
    }
    FrameStatistics Renderer::GetStatistics()
    {
        return *m_FrameStatistics;
    }

    // TODO: Think to move somewhere else...
    void Renderer::GenDefaultTexture()
    {
        uint32_t width = 1;
        uint32_t height = 1;
        TextureFormat format = TextureFormat::RGBA;
        std::vector<uint8_t> data(4, 255);

        TextureData tex_data(&data[0], width, height, format);
        m_DefaultTexture = Texture::Create(tex_data);
    }
    void Renderer::GenFullScreenQuad()
    {
        uint32_t indices[] = {0, 1, 2, 0, 2, 3};
        float vertices[] = {1.0f,
                            1.0f,
                            1.0f,
                            1.0f,  //

                            -1.0f,
                            1.0f,
                            0.0f,
                            1.0f,  //

                            -1.0f,
                            -1.0f,
                            0.0f,
                            0.0f,  //

                            1.0f,
                            -1.0f,
                            1.0f,
                            0.0f};
        auto vb = VertexBuffer::Create({BufferElementType::Float2, BufferElementType::Float2}, 4, vertices);
        auto ib = IndexBuffer::Create(indices, 6);
        m_FullScreenQuad = VertexArray::Create();
        m_FullScreenQuad->SetIndexBuffer(ib);
        m_FullScreenQuad->AddVertexBuffer(vb);
    }  // namespace DE

}  // namespace DE
