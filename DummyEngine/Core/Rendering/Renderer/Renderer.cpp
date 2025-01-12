#include "Renderer.h"

// TODO: Remove glad
#include <glad/glad.h>

#include "DummyEngine/Core/Application/Config.h"
#include "DummyEngine/Core/Objects/Cameras/FPSCamera.h"
#include "DummyEngine/Core/Rendering/Renderer/BufferLayout.h"
#include "DummyEngine/Core/Rendering/Renderer/FrameBuffer.h"
#include "DummyEngine/Core/Rendering/Renderer/RenderStructs.h"
#include "DummyEngine/Core/Rendering/Renderer/Shader.h"
#include "DummyEngine/Core/Rendering/RendererOpenGL/GLRenderAPI.h"

namespace DummyEngine {

    SINGLETON_BASE(Renderer);

    S_INITIALIZE() {
        switch (Config::Get().RendererAPI) {
            case API::OpenGL: render_api_ = CreateScope<GLRenderAPI>(); break;
            case API::Vulkan: {
                DE_ASSERT(false, "Attempt to init Renderer with VulkanAPI which is currently unsupported");
                render_api_ = nullptr;
                break;
            }
            case API::None: {
                DE_ASSERT(false, "Attempt to init Renderer without RenderingAPI");
                render_api_ = nullptr;
                break;
            }
        }
        render_api_->SetDefaultState();
        GenResources();
        LOG_INFO("Renderer initialized");
        return Unit();
    }
    S_TERMINATE() {
        return Unit();
    }

    S_METHOD_IMPL(Unit, SetViewport, (U32 width, U32 height), (width, height)) {
        render_api_->SetViewport(0, 0, width, height);
        return Unit();
    }
    S_METHOD_IMPL(Unit, BeginFrame, (), ()) {
        frame_statistics_.Reset();
        return Unit();
    }
    S_METHOD_IMPL(Unit, EndFrame, (), ()) {
        return Unit();
    }
    S_METHOD_IMPL(Unit, Clear, (), ()) {
        render_api_->Clear();
        return Unit();
    }

    S_METHOD_IMPL(Unit, Submit, (Ref<VertexArray> vertex_array, Ref<Shader> shader, const Mat4& transform), (vertex_array, shader, transform)) {
        shader->Bind();
        shader->SetMat4("u_Transform", transform);
        render_api_->DrawIndexed(vertex_array);

        ++frame_statistics_.DrawCallsAmount;
        ++frame_statistics_.DrawnInstances;
        return Unit();
    }
    S_METHOD_IMPL(Unit,
                  Submit,
                  (Ref<RenderMesh> mesh, Ref<Shader> shader, const Mat4& transform, const bool is_depthmap),
                  (mesh, shader, transform, is_depthmap)) {
        shader->Bind();
        shader->SetMat4("u_Transform", transform);
        for (const auto& sub_mesh : mesh->sub_meshes_) {
            if (!is_depthmap) {
                sub_mesh.Material.Apply(shader);
            }
            sub_mesh.VertexArray->Bind();
            render_api_->DrawIndexed(sub_mesh.VertexArray);

            ++frame_statistics_.DrawCallsAmount;
            ++frame_statistics_.DrawnInstances;
        }

        return Unit();
    }
    S_METHOD_IMPL(Unit, Submit, (Ref<CubeMap> cube_map, const FPSCamera& camera, const Mat4& transform), (cube_map, camera, transform)) {
        cube_map->Bind(11);
        auto shader = resources_.Skybox;
        shader->Bind();
        shader->SetInt("u_SkyBox", 11);
        shader->SetMat4("u_Transform", transform);
        shader->SetMat4("u_Projection", camera.GetProjectionMatrix());
        shader->SetMat4("u_View", camera.GetViewMatrix());
        shader->SetFloat("u_SkyBoxLOD", cube_map->GetLOD());
        render_api_->Disable(RenderSetting::DepthMask);
        render_api_->DrawIndexed(resources_.Cube);
        render_api_->Enable(RenderSetting::DepthMask);

        ++frame_statistics_.DrawCallsAmount;
        ++frame_statistics_.DrawnInstances;
        return Unit();
    }

    S_METHOD_IMPL(Unit,
                  Bloom,
                  (Ref<Texture> texture, float threshold, float soft_threshold, float radius, U32 depth, float strength),
                  (texture, threshold, soft_threshold, radius, depth, strength)) {
        Ref<FrameBuffer> result = FrameBuffer::Create({texture->GetWidth(), texture->GetHeight()});

        resources_.BrightnessFilter->Bind();
        resources_.BrightnessFilter->SetInt("u_Texture", 1);
        resources_.BrightnessFilter->SetFloat("u_Treshold", threshold);
        resources_.BrightnessFilter->SetFloat("u_SoftTreshold", soft_threshold);
        texture->Bind(1);
        result->AddColorAttachment(texture->GetFormat(), texture->GetChannels());
        result->Bind();
        SetViewport(texture->GetWidth(), texture->GetHeight());
        Clear();
        Submit(resources_.ScreenQuad, resources_.BrightnessFilter);
        Ref<Texture> bright_pixels = result->GetColorAttachment(0);

        Ref<Texture> down_up_samepled = BloomDownAndUpSample(bright_pixels, radius, depth);
        Ref<Texture> source           = Texture::Create();
        result->SetColorAttachment(texture, 0);
        source->Copy(result, 0);

        source->Bind(1);
        down_up_samepled->Bind(2);
        resources_.Bloom->Bind();
        resources_.Bloom->SetInt("u_Texture", 1);
        resources_.Bloom->SetInt("u_BrighnessTexture", 2);
        resources_.Bloom->SetFloat("u_Strength", strength);
        result->Bind();
        Renderer::SetViewport(texture->GetWidth(), texture->GetHeight());
        Renderer::Clear();
        Renderer::Submit(resources_.ScreenQuad, resources_.Bloom);
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
            LOG_WARNING("Failed to gen layers for Bloom because depth < 1 or texture is smaller then 2x2");
            return texture;
        }
        Ref<Texture> current = texture;
        height               = texture->GetHeight();
        width                = texture->GetWidth();
        for (U32 i = 0; i < layers.size(); ++i) {
            resources_.BloomDownsample->Bind();
            resources_.BloomDownsample->SetInt("u_Texture", 1);
            current->Bind(1);
            buffer->SetColorAttachment(layers[i], 0);
            buffer->Bind();
            Renderer::SetViewport(width / level_divisor, height / level_divisor);
            Renderer::Clear();
            Renderer::Submit(resources_.ScreenQuad, resources_.BloomDownsample);

            current = layers[i];
            height /= level_divisor;
            width /= level_divisor;
        }

        glBlendFunc(GL_ONE, GL_ONE);
        glBlendEquation(GL_FUNC_ADD);
        for (S32 i = layers.size() - 1; i > 0; --i) {
            Ref<Texture> small = layers[i];
            Ref<Texture> big   = layers[i - 1];
            resources_.BloomUpsample->Bind();
            resources_.BloomUpsample->SetInt("u_Texture", 1);
            resources_.BloomUpsample->SetFloat("u_Radius", radius);
            buffer->SetColorAttachment(big, 0);
            buffer->Bind();
            Renderer::SetViewport(big->GetWidth(), big->GetHeight());
            Renderer::Submit(resources_.ScreenQuad, resources_.BloomUpsample);
        }
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // Ref<Texture> result = Texture::Create(texture->GetWidth(), texture->GetHeight(), texture->GetChannels(), texture->GetFormat());
        // buffer->SetColorAttachment(result, 0);
        // buffer->Bind();
        // resources_.textured_quad->Bind();
        // resources_.textured_quad->SetInt("u_Texture", 1);
        // layers.front()->Bind(1);
        // Renderer::SetViewport(texture->GetWidth(), texture->GetHeight());
        // Renderer::Clear();
        // Renderer::Submit(resources_.ScreenQuad, resources_.textured_quad);

        return layers.front();
    }
    S_METHOD_IMPL(Unit, GammeHDRCorrecion, (Ref<Texture> texture, float exposure, float gamma), (texture, exposure, gamma)) {
        Ref<FrameBuffer> result = FrameBuffer::Create({texture->GetWidth(), texture->GetHeight()});
        result->AddColorAttachment(texture->GetFormat(), texture->GetChannels());

        result->Bind();
        resources_.GammaHDR->Bind();
        resources_.GammaHDR->SetInt("u_Texture", 1);
        resources_.GammaHDR->SetFloat("u_Exposure", exposure);
        resources_.GammaHDR->SetFloat("u_Gamma", gamma);
        texture->Bind(1);
        SetViewport(texture->GetWidth(), texture->GetHeight());
        Clear();
        Submit(resources_.ScreenQuad, resources_.GammaHDR);

        texture->Copy(result, 0);

        return Unit();
    }
    S_METHOD_IMPL(Unit, GaussianBlur, (Ref<Texture> texture), (texture)) {
        Ref<FrameBuffer> result = FrameBuffer::Create({texture->GetWidth(), texture->GetHeight()});
        result->AddColorAttachment(texture->GetFormat(), texture->GetChannels());

        result->Bind();
        texture->Bind(1);
        resources_.GaussianBlur->Bind();
        resources_.GaussianBlur->SetInt("u_Texture", 1);
        resources_.GaussianBlur->SetInt("u_Horizontal", 1);
        SetViewport(texture->GetWidth(), texture->GetHeight());
        Clear();
        Submit(resources_.ScreenQuad, resources_.GaussianBlur);

        Ref<Texture> horizontal = result->GetColorAttachment(0);

        result->SetColorAttachment(texture, 0);
        resources_.GaussianBlur->Bind();
        resources_.GaussianBlur->SetInt("u_Texture", 1);
        resources_.GaussianBlur->SetInt("u_Horizontal", 0);
        horizontal->Bind(1);
        result->Bind();
        SetViewport(texture->GetWidth(), texture->GetHeight());
        Clear();
        Submit(resources_.ScreenQuad, resources_.GaussianBlur);
        return Unit();
    }

    S_METHOD_IMPL(Unit, Enable, (RenderSetting setting), (setting)) {
        render_api_->Enable(setting);
        return Unit();
    }
    S_METHOD_IMPL(Unit, Disable, (RenderSetting setting), (setting)) {
        render_api_->Disable(setting);
        return Unit();
    }
    S_METHOD_IMPL(Unit, SetClearColor, (Vec4 color), (color)) {
        render_api_->SetClearColor(color);
        return Unit();
    }
    S_METHOD_IMPL(Unit, SetClearColor, (float r, float g, float b, float a), (r, g, b, a)) {
        render_api_->SetClearColor(Vec4(r, g, b, a));
        return Unit();
    }
    S_METHOD_IMPL(Unit, SetDefaultFrameBuffer, (), ()) {
        render_api_->SetDefaultFrameBuffer();
        return Unit();
    }

    S_METHOD_IMPL(Ref<Shader>, GetShader, (Shaders shader), (shader)) {
        switch (shader) {
            case Shaders::EquirectangularToCubeMap: return resources_.EquirectangularToCubemap;
            case Shaders::Skybox: return resources_.Skybox;
            case Shaders::Convolution: return resources_.Convolution;
            case Shaders::PreFileterConvolution: return resources_.PreFilterConvolution;
            case Shaders::BRDFConvolution: return resources_.BRDFConvolution;
            case Shaders::BrightnessFilter: return resources_.BrightnessFilter;
            case Shaders::TexturedQuad: return resources_.TexturedQuad;
            case Shaders::GaussianBlur: return resources_.GaussianBlur;
            case Shaders::Bloom: return resources_.Bloom;
            case Shaders::GammaHDR: return resources_.GammaHDR;
            case Shaders::BloomUpsample: return resources_.BloomUpsample;
            case Shaders::BloomDownsample: return resources_.BloomDownsample;
            case Shaders::DirectionalShadowMap: return resources_.DirectionalShadowMap;
            case Shaders::OmnidirectionalShadowMap: return resources_.OmnidirectionalShadowMap;
            case Shaders::Last:
            case Shaders::None: return nullptr;
            default: DE_ASSERT(false, "Wrong Renderer shader requested"); break;
        }
        return nullptr;
    }
    S_METHOD_IMPL(Ref<Texture>, GetTexture, (Textures texture), (texture)) {
        switch (texture) {
            case Textures::White: return resources_.White;
            case Textures::Normal: return resources_.Normal;
            case Textures::BRDF: return resources_.BRDF;
            case Textures::Last:
            case Textures::None: return nullptr;
            default: DE_ASSERT(false, "Wrong Renderer texture requested"); break;
        }
        return nullptr;
    }
    S_METHOD_IMPL(Ref<VertexArray>, GetVertexArray, (VertexArrays vao), (vao)) {
        switch (vao) {
            case VertexArrays::Cube: return resources_.Cube;
            case VertexArrays::ScreenQuad: return resources_.ScreenQuad;
            case VertexArrays::Last:
            case VertexArrays::None: return nullptr;
            default: DE_ASSERT(false, "Wrong Renderer vertex array requested"); break;
        }
        return nullptr;
    }

    S_METHOD_IMPL(API, CurrentAPI, (), ()) {
        return render_api_->GetAPI();
    }
    S_METHOD_IMPL(Renderer::Statistics, GetStatistics, (), ()) {
        return frame_statistics_;
    }
    S_METHOD_IMPL(RenderAPI&, GetRenderAPI, (), ()) {
        return *render_api_;
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
            resources_.White = Texture::Create(tex_data);
        }
        //*Normal
        {
            U32             width  = 1;
            U32             height = 1;
            TextureChannels format = TextureChannels::RGBA;
            std::vector<U8> data   = {128, 128, 255, 255};

            TextureData tex_data(&data[0], width, height, format);
            resources_.Normal = Texture::Create(tex_data);
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

            resources_.Cube = VertexArray::Create();
            resources_.Cube->SetIndexBuffer(ib);
            resources_.Cube->AddVertexBuffer(vb);
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

            resources_.ScreenQuad = VertexArray::Create();
            resources_.ScreenQuad->SetIndexBuffer(ib);
            resources_.ScreenQuad->AddVertexBuffer(vb);
        }

        //*___Shaders_____________________________________________________________________________________________________________________________________________________________________________
        Path shaders = Config::Get().ExecutablePath / "../DummyEngine/Core/Rendering/Shaders";
        //*EquirectangularToCubeMap
        {
            std::vector<ShaderPart> parts = {
                {  ShaderPartType::Vertex,                    shaders / "Vertex/CubeMap.vs"},
                {ShaderPartType::Fragment, shaders / "Fragment/EquirectangularToCubeMap.fs"},
            };

            resources_.EquirectangularToCubemap = Shader::Create(parts);
        }
        {
            std::vector<ShaderPart> parts = {
                {  ShaderPartType::Vertex,         shaders / "Vertex/CubeMap.vs"},
                {ShaderPartType::Fragment, shaders / "Fragment/CubeMapSample.fs"},
            };

            resources_.Skybox = Shader::Create(parts);
        }
        {
            std::vector<ShaderPart> parts = {
                {  ShaderPartType::Vertex,              shaders / "Vertex/CubeMap.vs"},
                {ShaderPartType::Fragment, shaders / "Fragment/CubeMapConvolution.fs"},
            };

            resources_.Convolution = Shader::Create(parts);
        }
        {
            std::vector<ShaderPart> parts = {
                {  ShaderPartType::Vertex,            shaders / "Vertex/CubeMap.vs"},
                {ShaderPartType::Fragment, shaders / "Fragment/CubeMapPrefilter.fs"},
            };

            resources_.PreFilterConvolution = Shader::Create(parts);
        }
        {
            std::vector<ShaderPart> parts = {
                {  ShaderPartType::Vertex,      shaders / "Vertex/TexturedQuad.vs"},
                {ShaderPartType::Fragment, shaders / "Fragment/BRDFConvolution.fs"},
            };

            resources_.BRDFConvolution = Shader::Create(parts);
        }
        {
            std::vector<ShaderPart> parts = {
                {  ShaderPartType::Vertex,              shaders / "Vertex/Bloom.vs"},
                {ShaderPartType::Fragment, shaders / "Fragment/BrightnessFilter.fs"}
            };

            resources_.BrightnessFilter = Shader::Create(parts);
        }
        {
            std::vector<ShaderPart> parts = {
                {  ShaderPartType::Vertex,   shaders / "Vertex/TexturedQuad.vs"},
                {ShaderPartType::Fragment, shaders / "Fragment/TexturedQuad.fs"}
            };

            resources_.TexturedQuad = Shader::Create(parts);
        }
        {
            std::vector<ShaderPart> parts = {
                {  ShaderPartType::Vertex,   shaders / "Vertex/TexturedQuad.vs"},
                {ShaderPartType::Fragment, shaders / "Fragment/GaussianBlur.fs"}
            };

            resources_.GaussianBlur = Shader::Create(parts);
        }
        {
            std::vector<ShaderPart> parts = {
                {  ShaderPartType::Vertex, shaders / "Vertex/TexturedQuad.vs"},
                {ShaderPartType::Fragment,      shaders / "Fragment/Bloom.fs"}
            };

            resources_.Bloom = Shader::Create(parts);
        }
        {
            std::vector<ShaderPart> parts = {
                {  ShaderPartType::Vertex, shaders / "Vertex/TexturedQuad.vs"},
                {ShaderPartType::Fragment,   shaders / "Fragment/GammaHDR.fs"}
            };

            resources_.GammaHDR = Shader::Create(parts);
        }
        {
            std::vector<ShaderPart> parts = {
                {  ShaderPartType::Vertex,    shaders / "Vertex/TexturedQuad.vs"},
                {ShaderPartType::Fragment, shaders / "Fragment/BloomUpsample.fs"}
            };

            resources_.BloomUpsample = Shader::Create(parts);
        }
        {
            std::vector<ShaderPart> parts = {
                {  ShaderPartType::Vertex,      shaders / "Vertex/TexturedQuad.vs"},
                {ShaderPartType::Fragment, shaders / "Fragment/BloomDownsample.fs"}
            };

            resources_.BloomDownsample = Shader::Create(parts);
        }
        {
            std::vector<ShaderPart> parts = {
                {  ShaderPartType::Vertex,   shaders / "Vertex/DirectionalShadowMap.vs"},
                {ShaderPartType::Fragment, shaders / "Fragment/DirectionalShadowMap.fs"}
            };

            resources_.DirectionalShadowMap = Shader::Create(parts);
        }
        {
            std::vector<ShaderPart> parts = {
                {  ShaderPartType::Vertex,   shaders / "Vertex/OmnidirectionalShadowMap.vs"},
                {ShaderPartType::Fragment, shaders / "Fragment/OmnidirectionalShadowMap.fs"},
                {ShaderPartType::Geometry, shaders / "Geometry/OmnidirectionalShadowMap.gs"}
            };

            resources_.OmnidirectionalShadowMap = Shader::Create(parts);
        }
        {
            const size_t     sz     = 1024;
            Ref<FrameBuffer> buffer = FrameBuffer::Create({sz, sz});
            buffer->Bind();
            buffer->SetDepthAttachment(Texture::Format::F32);
            buffer->AddColorAttachment(Texture::Format::U8, Texture::Channels::RG);
            SetViewport(sz, sz);
            Clear();
            Submit(resources_.ScreenQuad, resources_.BRDFConvolution);
            resources_.BRDF = buffer->GetColorAttachment(0);
        }
    }

    void Renderer::Statistics::Reset() {
        DrawCallsAmount = 0;
        DrawnInstances  = 0;
    }
}  // namespace DummyEngine