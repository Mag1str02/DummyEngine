#include "DummyEngine/Core/Rendering/Renderer/Renderer.h"

// TODO: Remove glad
#include <glad/glad.h>

#include "DummyEngine/Core/Application/Config.h"
#include "DummyEngine/Core/Rendering/Renderer/FrameBuffer.h"
#include "DummyEngine/Core/Rendering/RendererOpenGL/GLDebug.h"
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
    S_METHOD_IMPL(Unit, Submit, (Ref<RenderMesh> mesh, Ref<Shader> shader, const Mat4& transform, const bool is_depthmap), (mesh, shader, transform, is_depthmap)) {
        shader->Bind();
        shader->SetMat4("u_Transform", transform);
        for (const auto& sub_mesh : mesh->m_SubMeshes) {
            if (!is_depthmap) {
                sub_mesh.material.Apply(shader);
            }
            sub_mesh.vertex_array->Bind();
            m_RenderAPI->DrawIndexed(sub_mesh.vertex_array);

            ++m_FrameStatistics.m_DrawCallsAmount;
            ++m_FrameStatistics.m_DrawnInstances;
        }

        return Unit();
    }
    S_METHOD_IMPL(Unit, Submit, (Ref<CubeMap> cube_map, const FPSCamera& camera, const Mat4& transform), (cube_map, camera, transform)) {
        cube_map->Bind(11);
        auto shader = m_Resources.skybox;
        shader->Bind();
        shader->SetInt("u_SkyBox", 11);
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

    S_METHOD_IMPL(Unit,
                  Bloom,
                  (Ref<Texture> texture, float threshold, float soft_threshold, float radius, U32 depth, float strength),
                  (texture, threshold, soft_threshold, radius, depth, strength)) {
        Ref<FrameBuffer> result = FrameBuffer::Create({texture->GetWidth(), texture->GetHeight()});

        m_Resources.brightness_filter->Bind();
        m_Resources.brightness_filter->SetInt("u_Texture", 1);
        m_Resources.brightness_filter->SetFloat("u_Treshold", threshold);
        m_Resources.brightness_filter->SetFloat("u_SoftTreshold", soft_threshold);
        texture->Bind(1);
        result->AddColorAttachment(texture->GetFormat(), texture->GetChannels());
        result->Bind();
        SetViewport(texture->GetWidth(), texture->GetHeight());
        Clear();
        Submit(m_Resources.screen_quad, m_Resources.brightness_filter);
        Ref<Texture> bright_pixels = result->GetColorAttachment(0);

        Ref<Texture> down_up_samepled = BloomDownAndUpSample(bright_pixels, radius, depth);
        Ref<Texture> source           = Texture::Create();
        result->SetColorAttachment(texture, 0);
        source->Copy(result, 0);

        source->Bind(1);
        down_up_samepled->Bind(2);
        m_Resources.bloom->Bind();
        m_Resources.bloom->SetInt("u_Texture", 1);
        m_Resources.bloom->SetInt("u_BrighnessTexture", 2);
        m_Resources.bloom->SetFloat("u_Strength", strength);
        result->Bind();
        Renderer::SetViewport(texture->GetWidth(), texture->GetHeight());
        Renderer::Clear();
        Renderer::Submit(m_Resources.screen_quad, m_Resources.bloom);
        return Unit();
    }
    S_METHOD_IMPL(Ref<Texture>, BloomDownAndUpSample, (Ref<Texture> texture, float radius, U32 depth), (texture, radius, depth)) {
        const float               level_divisor = 1.5;
        U32                       height        = texture->GetHeight() / level_divisor;
        U32                       width         = texture->GetWidth() / level_divisor;
        Ref<FrameBuffer>          buffer        = FrameBuffer::Create({width, height});
        std::vector<Ref<Texture>> layers;
        while (height > 0 && width > 0 && layers.size() < depth) {
            layers.push_back(Texture::Create(width, height, texture->GetChannels(), texture->GetFormat()));
            height /= level_divisor;
            width /= level_divisor;
        }
        if (layers.empty()) {
            LOG_WARNING("Renderer", "Failed to gen layers for bloom because depth < 1 or texture is smaller then 2x2");
            return texture;
        }
        Ref<Texture> current = texture;
        height               = texture->GetHeight();
        width                = texture->GetWidth();
        for (U32 i = 0; i < layers.size(); ++i) {
            m_Resources.bloom_downsample->Bind();
            m_Resources.bloom_downsample->SetInt("u_Texture", 1);
            current->Bind(1);
            buffer->SetColorAttachment(layers[i], 0);
            buffer->Bind();
            Renderer::SetViewport(width / level_divisor, height / level_divisor);
            Renderer::Clear();
            Renderer::Submit(m_Resources.screen_quad, m_Resources.bloom_downsample);

            current = layers[i];
            height /= level_divisor;
            width /= level_divisor;
        }

        glBlendFunc(GL_ONE, GL_ONE);
        glBlendEquation(GL_FUNC_ADD);
        for (S32 i = layers.size() - 1; i > 0; --i) {
            Ref<Texture> small = layers[i];
            Ref<Texture> big   = layers[i - 1];
            m_Resources.bloom_upsample->Bind();
            m_Resources.bloom_upsample->SetInt("u_Texture", 1);
            m_Resources.bloom_upsample->SetFloat("u_Radius", radius);
            buffer->SetColorAttachment(big, 0);
            buffer->Bind();
            Renderer::SetViewport(big->GetWidth(), big->GetHeight());
            Renderer::Submit(m_Resources.screen_quad, m_Resources.bloom_upsample);
        }
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // Ref<Texture> result = Texture::Create(texture->GetWidth(), texture->GetHeight(), texture->GetChannels(), texture->GetFormat());
        // buffer->SetColorAttachment(result, 0);
        // buffer->Bind();
        // m_Resources.textured_quad->Bind();
        // m_Resources.textured_quad->SetInt("u_Texture", 1);
        // layers.front()->Bind(1);
        // Renderer::SetViewport(texture->GetWidth(), texture->GetHeight());
        // Renderer::Clear();
        // Renderer::Submit(m_Resources.screen_quad, m_Resources.textured_quad);

        return layers.front();
    }
    S_METHOD_IMPL(Unit, GammeHDRCorrecion, (Ref<Texture> texture, float exposure, float gamma), (texture, exposure, gamma)) {
        Ref<FrameBuffer> result = FrameBuffer::Create({texture->GetWidth(), texture->GetHeight()});
        result->AddColorAttachment(texture->GetFormat(), texture->GetChannels());

        result->Bind();
        m_Resources.gamma_hdr->Bind();
        m_Resources.gamma_hdr->SetInt("u_Texture", 1);
        m_Resources.gamma_hdr->SetFloat("u_Exposure", exposure);
        m_Resources.gamma_hdr->SetFloat("u_Gamma", gamma);
        texture->Bind(1);
        SetViewport(texture->GetWidth(), texture->GetHeight());
        Clear();
        Submit(m_Resources.screen_quad, m_Resources.gamma_hdr);

        texture->Copy(result, 0);

        return Unit();
    }
    S_METHOD_IMPL(Unit, GaussianBlur, (Ref<Texture> texture), (texture)) {
        Ref<FrameBuffer> result = FrameBuffer::Create({texture->GetWidth(), texture->GetHeight()});
        result->AddColorAttachment(texture->GetFormat(), texture->GetChannels());

        result->Bind();
        texture->Bind(1);
        m_Resources.gaussian_blur->Bind();
        m_Resources.gaussian_blur->SetInt("u_Texture", 1);
        m_Resources.gaussian_blur->SetInt("u_Horizontal", 1);
        SetViewport(texture->GetWidth(), texture->GetHeight());
        Clear();
        Submit(m_Resources.screen_quad, m_Resources.gaussian_blur);

        Ref<Texture> horizontal = result->GetColorAttachment(0);

        result->SetColorAttachment(texture, 0);
        m_Resources.gaussian_blur->Bind();
        m_Resources.gaussian_blur->SetInt("u_Texture", 1);
        m_Resources.gaussian_blur->SetInt("u_Horizontal", 0);
        horizontal->Bind(1);
        result->Bind();
        SetViewport(texture->GetWidth(), texture->GetHeight());
        Clear();
        Submit(m_Resources.screen_quad, m_Resources.gaussian_blur);
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
            case Shaders::TexturedQuad: return m_Resources.textured_quad;
            case Shaders::GaussianBlur: return m_Resources.gaussian_blur;
            case Shaders::Bloom: return m_Resources.bloom;
            case Shaders::GammaHDR: return m_Resources.gamma_hdr;
            case Shaders::BloomUpsample: return m_Resources.bloom_upsample;
            case Shaders::BloomDownsample: return m_Resources.bloom_downsample;
            case Shaders::DirectionalShadowMap: return m_Resources.directional_shadow_map;
            case Shaders::OmnidirectionalShadowMap: return m_Resources.omnidirectional_shadow_map;
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
                {  ShaderPartType::Vertex,      shaders / "Vertex/TexturedQuad.vs"},
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

            m_Resources.textured_quad = Shader::Create(parts);
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
                {  ShaderPartType::Vertex, shaders / "Vertex/TexturedQuad.vs"},
                {ShaderPartType::Fragment,      shaders / "Fragment/Bloom.fs"}
            };

            m_Resources.bloom = Shader::Create(parts);
        }
        {
            std::vector<ShaderPart> parts = {
                {  ShaderPartType::Vertex, shaders / "Vertex/TexturedQuad.vs"},
                {ShaderPartType::Fragment,   shaders / "Fragment/GammaHDR.fs"}
            };

            m_Resources.gamma_hdr = Shader::Create(parts);
        }
        {
            std::vector<ShaderPart> parts = {
                {  ShaderPartType::Vertex,    shaders / "Vertex/TexturedQuad.vs"},
                {ShaderPartType::Fragment, shaders / "Fragment/BloomUpsample.fs"}
            };

            m_Resources.bloom_upsample = Shader::Create(parts);
        }
        {
            std::vector<ShaderPart> parts = {
                {  ShaderPartType::Vertex,      shaders / "Vertex/TexturedQuad.vs"},
                {ShaderPartType::Fragment, shaders / "Fragment/BloomDownsample.fs"}
            };

            m_Resources.bloom_downsample = Shader::Create(parts);
        }
        {
            std::vector<ShaderPart> parts = {
                {  ShaderPartType::Vertex,      shaders / "Vertex/DirectionalShadowMap.vs"},
                {ShaderPartType::Fragment, shaders / "Fragment/DirectionalShadowMap.fs"}
            };

            m_Resources.directional_shadow_map = Shader::Create(parts);
        }
        {
            std::vector<ShaderPart> parts = {
                {ShaderPartType::Vertex,   shaders / "Vertex/OmnidirectionalShadowMap.vs"},
                {ShaderPartType::Fragment, shaders / "Fragment/OmnidirectionalShadowMap.fs"},
                {ShaderPartType::Geometry, shaders / "Geometry/OmnidirectionalShadowMap.gs"}
            };

            m_Resources.omnidirectional_shadow_map = Shader::Create(parts);
        }
        {
            const size_t     sz     = 1024;
            Ref<FrameBuffer> buffer = FrameBuffer::Create({sz, sz});
            buffer->Bind();
            buffer->SetDepthAttachment(Texture::Format::F32);
            buffer->AddColorAttachment(Texture::Format::U8, Texture::Channels::RG);
            SetViewport(sz, sz);
            Clear();
            Submit(m_Resources.screen_quad, m_Resources.brdf_convolution);
            m_Resources.brdf = buffer->GetColorAttachment(0);
        }
    }

    void Renderer::Statistics::Reset() {
        m_DrawCallsAmount = 0;
        m_DrawnInstances  = 0;
    }
}  // namespace DE