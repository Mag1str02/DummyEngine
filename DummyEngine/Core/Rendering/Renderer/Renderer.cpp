#include "DummyEngine/Core/Rendering/Renderer/Renderer.h"

#include <glad/glad.h>

#include "DummyEngine/Core/Application/Config.h"
#include "DummyEngine/Core/Rendering/RendererOpenGL/GLRenderAPI.h"
#include "DummyEngine/Core/ResourceManaging/ResourceManager.h"
#include "DummyEngine/ToolBox/Loaders/TextureLoader.h"

namespace DE {
    Scope<FrameStatistics> Renderer::m_FrameStatistics = nullptr;
    Scope<RenderAPI>       Renderer::m_RenderAPI       = nullptr;
    Ref<Texture>           Renderer::m_DefaultTexture  = nullptr;
    Ref<VertexArray>       Renderer::m_FullScreenQuad  = nullptr;
    Ref<VertexArray>       Renderer::m_Cube            = nullptr;

    void FrameStatistics::Reset() {
        m_DrawCallsAmount = 0;
        m_DrawnInstances  = 0;
    }

    void Renderer::Initialize() {
        m_FrameStatistics = CreateScope<FrameStatistics>();
        switch (Config::GetRenderAPI()) {
            case API::OpenGL: m_RenderAPI = CreateScope<GLRenderAPI>(); break;
            case API::Vulkan: {
                DE_ASSERT(false, "Attempt to init Renderer with VulkanAPI which is currently unsupported");
                m_RenderAPI = nullptr;
                break;
            }
            case API::None: {
                DE_ASSERT(false, "Attempt to init Renderer without RenderingAPI");
                m_RenderAPI = nullptr;
                break;
            }
        }

        m_RenderAPI->SetDefaultState();

        GenDefaultTexture();
        GenFullScreenQuad();
        GenCube();

        LOG_INFO("Renderer", "Renderer initialized");
    }
    void Renderer::Terminate() {}

    void Renderer::OnWindowResize(U32 width, U32 height) {
        m_RenderAPI->SetViewport(0, 0, width, height);
    }

    void Renderer::BeginFrame() {
        m_FrameStatistics->Reset();
    }
    void Renderer::EndFrame() {}

    void Renderer::Clear() {
        m_RenderAPI->Clear();
    }
    void Renderer::Submit(const Ref<VertexArray>& vertex_array, Ref<Shader> shader, const Mat4& trasform) {
        shader->Bind();
        shader->SetMat4("u_Transform", trasform);
        vertex_array->Bind();
        m_RenderAPI->DrawIndexed(vertex_array);

        ++m_FrameStatistics->m_DrawCallsAmount;
        ++m_FrameStatistics->m_DrawnInstances;
    }
    void Renderer::Submit(Ref<RenderMesh> mesh, Ref<Shader> shader, const Mat4& trasform) {
        shader->Bind();
        if (mesh->m_InstanceBuffer) {
            shader->SetInt("u_Instanced", 1);
            for (const auto& sub_mesh : mesh->m_SubMeshes) {
                shader->SetMaterial("u_Material", sub_mesh.material);
                sub_mesh.vertex_array->Bind();
                m_RenderAPI->DrawInstanced(sub_mesh.vertex_array, mesh->m_Instances.size());

                ++m_FrameStatistics->m_DrawCallsAmount;
                m_FrameStatistics->m_DrawnInstances += mesh->m_Instances.size();
            }
        } else {
            shader->SetInt("u_Instanced", 0);
            shader->SetMat4("u_Transform", trasform);
            for (const auto& sub_mesh : mesh->m_SubMeshes) {
                shader->SetMaterial("u_Material", sub_mesh.material);
                sub_mesh.vertex_array->Bind();
                m_RenderAPI->DrawInstanced(sub_mesh.vertex_array, 1);

                ++m_FrameStatistics->m_DrawCallsAmount;
                ++m_FrameStatistics->m_DrawnInstances;
            }
        }
    }
    void Renderer::Submit(Ref<CubeMap> cube_map, Ref<Shader> shader, const Mat4& trasform) {
        cube_map->Bind();
        shader->Bind();
        shader->SetMat4("u_Transform", trasform);
        m_RenderAPI->DrawIndexed(GetCube());

        ++m_FrameStatistics->m_DrawCallsAmount;
        ++m_FrameStatistics->m_DrawnInstances;
    }

    void Renderer::Enable(RenderSetting setting) {
        m_RenderAPI->Enable(setting);
    }
    void Renderer::Disable(RenderSetting setting) {
        m_RenderAPI->Disable(setting);
    }

    void Renderer::SetClearColor(Vec4 color) {
        m_RenderAPI->SetClearColor(color);
    }
    void Renderer::SetClearColor(float r, float g, float b, float a) {
        m_RenderAPI->SetClearColor(Vec4(r, g, b, a));
    }

    Ref<Texture> Renderer::GetDefaultTexture() {
        return m_DefaultTexture;
    }
    Ref<VertexArray> Renderer::GetFullScreenQuad() {
        return m_FullScreenQuad;
    }
    Ref<VertexArray> Renderer::GetCube() {
        return m_Cube;
    }
    API Renderer::CurrentAPI() {
        return m_RenderAPI->GetAPI();
    }
    FrameStatistics Renderer::GetStatistics() {
        return *m_FrameStatistics;
    }
    RenderAPI& Renderer::GetRenderAPI() {
        return *m_RenderAPI;
    }

    // TODO: Think to move somewhere else...

    void Renderer::GenDefaultTexture() {
        U32             width  = 1;
        U32             height = 1;
        TextureFormat   format = TextureFormat::RGBA;
        std::vector<U8> data(4, 255);

        TextureData tex_data(&data[0], width, height, format);
        m_DefaultTexture = Texture::Create(tex_data);
    }
    void Renderer::GenFullScreenQuad() {
        U32 indices[] = {
            0,
            1,
            2,  //
            0,
            2,
            3,  //
        };
        float vertices[] = {
            1.0f,
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
            0.0f,  //
        };

        auto vb = VertexBuffer::Create({BufferElementType::Float2, BufferElementType::Float2}, 4, vertices);
        auto ib = IndexBuffer::Create(indices, 6);

        m_FullScreenQuad = VertexArray::Create();
        m_FullScreenQuad->SetIndexBuffer(ib);
        m_FullScreenQuad->AddVertexBuffer(vb);
    }
    void Renderer::GenCube() {
        U32 indices[] = {
            0, 1, 2,  //
            2, 3, 0,  //

            1, 5, 6,  //
            6, 2, 1,  //

            7, 6, 5,  //
            5, 4, 7,  //

            4, 0, 3,  //
            3, 7, 4,  //

            4, 5, 1,  //
            1, 0, 4,  //

            3, 2, 6,  //
            6, 7, 3   //
        };
        float vertices[] = {

            -1.0, -1.0, 1.0,  //
            1.0,  -1.0, 1.0,  //
            1.0,  1.0,  1.0,  //
            -1.0, 1.0,  1.0,  //

            -1.0, -1.0, -1.0,  //
            1.0,  -1.0, -1.0,  //
            1.0,  1.0,  -1.0,  //
            -1.0, 1.0,  -1.0,  //
        };

        auto ib = IndexBuffer::Create(indices, 36);
        auto vb = VertexBuffer::Create({{BufferElementType::Float3}, 0}, 8, vertices);

        m_Cube = VertexArray::Create();
        m_Cube->SetIndexBuffer(ib);
        m_Cube->AddVertexBuffer(vb);
    }
}  // namespace DE
