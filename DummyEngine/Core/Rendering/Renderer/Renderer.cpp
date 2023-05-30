#include "DummyEngine/Core/Rendering/Renderer/Renderer.h"

#include <glad/glad.h>

#include "DummyEngine/Core/Application/Config.h"
#include "DummyEngine/Core/Rendering/Renderer/FrameBuffer.h"
#include "DummyEngine/Core/Rendering/RendererOpenGL/GLRenderAPI.h"
#include "DummyEngine/Core/ResourceManaging/ResourceManager.h"
#include "DummyEngine/ToolBox/Loaders/TextureLoader.h"

namespace DE {

    SINGLETON_BASE(Renderer);

    S_INITIALIZE() {
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
        GenResources();
        LOG_INFO("Renderer", "Renderer initialized");
        return Unit();
    }
    S_TERMINATE() {
        return Unit();
    }

    S_METHOD_IMPL(Unit, SetViewport, (U32 width, U32 height), (width, height)) {
        m_RenderAPI->SetViewport(0, 0, width, height);
        return Unit();
    }
    S_METHOD_IMPL(Unit, BeginFrame, (), ()) {
        m_FrameStatistics.Reset();
        return Unit();
    }
    S_METHOD_IMPL(Unit, EndFrame, (), ()) {
        return Unit();
    }
    S_METHOD_IMPL(Unit, Clear, (), ()) {
        m_RenderAPI->Clear();
        return Unit();
    }

    S_METHOD_IMPL(Unit, Submit, (Ref<VertexArray> vertex_array, Ref<Shader> shader, const Mat4& transform), (vertex_array, shader, transform)) {
        shader->Bind();
        shader->SetMat4("u_Transform", transform);
        m_RenderAPI->DrawIndexed(vertex_array);

        ++m_FrameStatistics.m_DrawCallsAmount;
        ++m_FrameStatistics.m_DrawnInstances;
        return Unit();
    }
    S_METHOD_IMPL(Unit, Submit, (Ref<RenderMesh> mesh, Ref<Shader> shader, const Mat4& transform), (mesh, shader, transform)) {
        shader->Bind();
        if (mesh->m_InstanceBuffer) {
            shader->SetInt("u_Instanced", 1);
            for (const auto& sub_mesh : mesh->m_SubMeshes) {
                sub_mesh.material.Apply(shader);
                sub_mesh.vertex_array->Bind();
                m_RenderAPI->DrawInstanced(sub_mesh.vertex_array, mesh->m_Instances.size());

                ++m_FrameStatistics.m_DrawCallsAmount;
                m_FrameStatistics.m_DrawnInstances += mesh->m_Instances.size();
            }
        } else {
            shader->SetInt("u_Instanced", 0);
            shader->SetMat4("u_Transform", transform);
            for (const auto& sub_mesh : mesh->m_SubMeshes) {
                sub_mesh.material.Apply(shader);
                sub_mesh.vertex_array->Bind();
                m_RenderAPI->DrawInstanced(sub_mesh.vertex_array, 1);

                ++m_FrameStatistics.m_DrawCallsAmount;
                ++m_FrameStatistics.m_DrawnInstances;
            }
        }
        return Unit();
    }
    S_METHOD_IMPL(Unit, Submit, (Ref<CubeMap> cube_map, const FPSCamera& camera, const Mat4& transform), (cube_map, camera, transform)) {
        cube_map->Bind(5);
        auto shader = m_Resources.skybox;
        shader->Bind();
        shader->SetInt("u_SkyBox", 5);
        shader->SetMat4("u_Transform", transform);
        shader->SetMat4("u_Projection", camera.GetProjectionMatrix());
        shader->SetMat4("u_View", camera.GetViewMatrix());
        shader->SetFloat("u_SkyBoxLOD", cube_map->GetLOD());
        m_RenderAPI->Disable(RenderSetting::DepthMask);
        m_RenderAPI->DrawIndexed(m_Resources.cube);
        m_RenderAPI->Enable(RenderSetting::DepthMask);

        ++m_FrameStatistics.m_DrawCallsAmount;
        ++m_FrameStatistics.m_DrawnInstances;
        return Unit();
    }

    S_METHOD_IMPL(Unit, Enable, (RenderSetting setting), (setting)) {
        m_RenderAPI->Enable(setting);
        return Unit();
    }
    S_METHOD_IMPL(Unit, Disable, (RenderSetting setting), (setting)) {
        m_RenderAPI->Disable(setting);
        return Unit();
    }
    S_METHOD_IMPL(Unit, SetClearColor, (Vec4 color), (color)) {
        m_RenderAPI->SetClearColor(color);
        return Unit();
    }
    S_METHOD_IMPL(Unit, SetClearColor, (float r, float g, float b, float a), (r, g, b, a)) {
        m_RenderAPI->SetClearColor(Vec4(r, g, b, a));
        return Unit();
    }
    S_METHOD_IMPL(Unit, SetDefaultFrameBuffer, (), ()) {
        m_RenderAPI->SetDefaultFrameBuffer();
        return Unit();
    }

    S_METHOD_IMPL(Ref<Shader>, GetShader, (Shaders shader), (shader)) {
        switch (shader) {
            case Shaders::EquirectangularToCubeMap: return m_Resources.equirectangular_to_cubemap;
            case Shaders::Skybox: return m_Resources.skybox;
            case Shaders::Convolution: return m_Resources.convolution;
            case Shaders::PreFileterConvolution: return m_Resources.pre_filter_convolution;
            case Shaders::BRDFConvolution: return m_Resources.brdf_convolution;
            case Shaders::BrightnessFilter: return m_Resources.brightness_filter;
            case Shaders::TexturedQuad: return m_Resources.copy_texture;
            case Shaders::GaussianBlur: return m_Resources.gaussian_blur;
            case Shaders::Bloom: return m_Resources.bloom;
            case Shaders::Last:
            case Shaders::None: return nullptr;
            default: DE_ASSERT(false, "Wrong Renderer shader requested"); break;
        }
        return nullptr;
    }
    S_METHOD_IMPL(Ref<Texture>, GetTexture, (Textures texture), (texture)) {
        switch (texture) {
            case Textures::White: return m_Resources.white;
            case Textures::Normal: return m_Resources.normal;
            case Textures::BRDF: return m_Resources.brdf;
            case Textures::Last:
            case Textures::None: return nullptr;
            default: DE_ASSERT(false, "Wrong Renderer texture requested"); break;
        }
        return nullptr;
    }
    S_METHOD_IMPL(Ref<VertexArray>, GetVertexArray, (VertexArrays vao), (vao)) {
        switch (vao) {
            case VertexArrays::Cube: return m_Resources.cube;
            case VertexArrays::ScreenQuad: return m_Resources.screen_quad;
            case VertexArrays::Last:
            case VertexArrays::None: return nullptr;
            default: DE_ASSERT(false, "Wrong Renderer vertex array requested"); break;
        }
        return nullptr;
    }

    S_METHOD_IMPL(API, CurrentAPI, (), ()) {
        return m_RenderAPI->GetAPI();
    }
    S_METHOD_IMPL(Renderer::Statistics, GetStatistics, (), ()) {
        return m_FrameStatistics;
    }
    S_METHOD_IMPL(RenderAPI&, GetRenderAPI, (), ()) {
        return *m_RenderAPI;
    }

    void Renderer::GenResources() {
        //*___Textures_________________________________________________________________________________________________________________________________________________________________________________
        //*White
        {
            U32             width  = 1;
            U32             height = 1;
            TextureChannels format = TextureChannels::RGBA;
            std::vector<U8> data(4, 255);

            TextureData tex_data(&data[0], width, height, format);
            m_Resources.white = Texture::Create(tex_data);
        }
        //*Normal
        {
            U32             width  = 1;
            U32             height = 1;
            TextureChannels format = TextureChannels::RGBA;
            std::vector<U8> data   = {128, 128, 255, 255};

            TextureData tex_data(&data[0], width, height, format);
            m_Resources.normal = Texture::Create(tex_data);
        }

        //*___VertexArrays_____________________________________________________________________________________________________________________________________________________________________________
        //*Cube
        {
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

            m_Resources.cube = VertexArray::Create();
            m_Resources.cube->SetIndexBuffer(ib);
            m_Resources.cube->AddVertexBuffer(vb);
        }
        //*ScreenQuad
        {
            U32 indices[] = {
                2,
                1,
                0,
                0,
                2,
                3,
            };
            float vertices[] = {
                1.0f,
                1.0f,
                1.0f,
                1.0f,

                -1.0f,
                1.0f,
                0.0f,
                1.0f,

                -1.0f,
                -1.0f,
                0.0f,
                0.0f,

                1.0f,
                -1.0f,
                1.0f,
                0.0f,
            };

            auto vb = VertexBuffer::Create({BufferElementType::Float2, BufferElementType::Float2}, 4, vertices);
            auto ib = IndexBuffer::Create(indices, 6);

            m_Resources.screen_quad = VertexArray::Create();
            m_Resources.screen_quad->SetIndexBuffer(ib);
            m_Resources.screen_quad->AddVertexBuffer(vb);
        }

        //*___Shaders_____________________________________________________________________________________________________________________________________________________________________________
        Path shaders = Config::GetPath(DE_CFG_EXECUTABLE_PATH) / "../DummyEngine/Core/Rendering/Shaders";
        //*EquirectangularToCubeMap
        {
            std::vector<ShaderPart> parts = {
                {  ShaderPartType::Vertex,                    shaders / "Vertex/CubeMap.vs"},
                {ShaderPartType::Fragment, shaders / "Fragment/EquirectangularToCubeMap.fs"},
            };

            m_Resources.equirectangular_to_cubemap = Shader::Create(parts);
        }
        {
            std::vector<ShaderPart> parts = {
                {  ShaderPartType::Vertex,         shaders / "Vertex/CubeMap.vs"},
                {ShaderPartType::Fragment, shaders / "Fragment/CubeMapSample.fs"},
            };

            m_Resources.skybox = Shader::Create(parts);
        }
        {
            std::vector<ShaderPart> parts = {
                {  ShaderPartType::Vertex,              shaders / "Vertex/CubeMap.vs"},
                {ShaderPartType::Fragment, shaders / "Fragment/CubeMapConvolution.fs"},
            };

            m_Resources.convolution = Shader::Create(parts);
        }
        {
            std::vector<ShaderPart> parts = {
                {  ShaderPartType::Vertex,            shaders / "Vertex/CubeMap.vs"},
                {ShaderPartType::Fragment, shaders / "Fragment/CubeMapPrefilter.fs"},
            };

            m_Resources.pre_filter_convolution = Shader::Create(parts);
        }
        {
            std::vector<ShaderPart> parts = {
                {  ShaderPartType::Vertex,          shaders / "Vertex/Square2D.vs"},
                {ShaderPartType::Fragment, shaders / "Fragment/BRDFConvolution.fs"},
            };

            m_Resources.brdf_convolution = Shader::Create(parts);
        }
        {
            std::vector<ShaderPart> parts = {
                    {  ShaderPartType::Vertex,              shaders / "Vertex/Bloom.vs"},
                    {ShaderPartType::Fragment, shaders / "Fragment/BrightnessFilter.fs"}
            };

            m_Resources.brightness_filter = Shader::Create(parts);
        }
        {
            std::vector<ShaderPart> parts = {
                    {  ShaderPartType::Vertex,   shaders / "Vertex/TexturedQuad.vs"},
                    {ShaderPartType::Fragment, shaders / "Fragment/TexturedQuad.fs"}
            };

            m_Resources.copy_texture = Shader::Create(parts);
        }
        {
            std::vector<ShaderPart> parts = {
                    {  ShaderPartType::Vertex,   shaders / "Vertex/TexturedQuad.vs"},
                    {ShaderPartType::Fragment, shaders / "Fragment/GaussianBlur.fs"}
            };

            m_Resources.gaussian_blur = Shader::Create(parts);
        }
        {
            std::vector<ShaderPart> parts = {
                    {  ShaderPartType::Vertex,   shaders / "Vertex/TexturedQuad.vs"},
                    {ShaderPartType::Fragment, shaders / "Fragment/Bloom.fs"}
            };

            m_Resources.bloom = Shader::Create(parts);
        }

        {
            const size_t     sz          = 1024;
            Ref<Shader>      brdf_shader = Renderer::GetShader(Renderer::Shaders::BRDFConvolution);
            Ref<FrameBuffer> buffer      = FrameBuffer::Create({sz, sz});
            Ref<VertexArray> quad        = Renderer::GetVertexArray(Renderer::VertexArrays::ScreenQuad);
            buffer->Bind();
            buffer->SetDepthAttachment(TextureChannels::Depth);
            buffer->AddColorAttachment(TextureChannels::RG);
            Renderer::SetViewport(sz, sz);
            Renderer::Clear();
            Renderer::Submit(quad, brdf_shader);
            m_Resources.brdf = buffer->GetColorAttachment(0);
        }
    }

    void Renderer::Statistics::Reset() {
        m_DrawCallsAmount = 0;
        m_DrawnInstances  = 0;
    }
}  // namespace DE