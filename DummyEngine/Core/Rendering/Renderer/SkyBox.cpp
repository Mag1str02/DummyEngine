#include "SkyBox.h"

#include "DummyEngine/Core/Rendering/Renderer/CubeMap.h"
#include "DummyEngine/Core/Rendering/Renderer/FrameBuffer.h"
#include "DummyEngine/Core/Rendering/Renderer/Renderer.h"
#include "DummyEngine/Core/Rendering/Renderer/Shader.h"

namespace DummyEngine {
    constexpr U32 kSkyboxSize            = 2048;
    constexpr U32 kIrradianceTextureUnit = 7;
    constexpr U32 kPrefilterTextureUnit  = 8;
    constexpr U32 kBRDFTextureUnit       = 9;
    constexpr U32 kMaxLevels             = 10;

    SkyBox::SkyBox(Ref<TextureData> texture) {
        raw_ = GenRawCubeMap(texture);
        BakeIBL();
    }
    SkyBox::SkyBox(Ref<CubeMap> cubemap) {
        raw_ = cubemap;
        BakeIBL();
    }

    Ref<CubeMap> SkyBox::GetMap() {
        return raw_;
    }
    void SkyBox::ApplyIBL(Ref<Shader> shader) {
        if (!irradiance_) {
            return;
        }
        if (!prefilter_) {
            return;
        }
        irradiance_->Bind(kIrradianceTextureUnit);
        prefilter_->Bind(kPrefilterTextureUnit);
        Renderer::GetTexture(Renderer::Textures::BRDF)->Bind(kBRDFTextureUnit);
        shader->SetInt("u_IrradianceMap", kIrradianceTextureUnit);
        shader->SetInt("u_PrefilterMap", kPrefilterTextureUnit);
        shader->SetInt("u_BRDF", kBRDFTextureUnit);
    }

    Ref<CubeMap> SkyBox::GenRawCubeMap(Ref<TextureData> texture) {
        Ref<Texture>     tex    = Texture::Create(*texture);
        Ref<CubeMap>     res    = CubeMap::Create(kSkyboxSize, Texture::DataFormat(texture->Format()), Texture::DataChannels(texture->Channels()));
        Ref<Shader>      shader = Renderer::GetShader(Renderer::Shaders::EquirectangularToCubeMap);
        Ref<VertexArray> cube   = Renderer::GetVertexArray(Renderer::VertexArrays::Cube);

        Mat4 projection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
        Mat4 views[]    = {glm::lookAt(Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f), Vec3(0.0f, -1.0f, 0.0f)),
                           glm::lookAt(Vec3(0.0f, 0.0f, 0.0f), Vec3(-1.0f, 0.0f, 0.0f), Vec3(0.0f, -1.0f, 0.0f)),
                           glm::lookAt(Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f), Vec3(0.0f, 0.0f, 1.0f)),
                           glm::lookAt(Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, -1.0f, 0.0f), Vec3(0.0f, 0.0f, -1.0f)),
                           glm::lookAt(Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 1.0f), Vec3(0.0f, -1.0f, 0.0f)),
                           glm::lookAt(Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, -1.0f), Vec3(0.0f, -1.0f, 0.0f))};

        shader->Bind();
        shader->SetInt("u_EquirectangularMap", 1);
        shader->SetMat4("u_Projection", projection);
        for (U32 i = 0; i < 6; ++i) {
            shader->SetMat4("u_View", views[i]);
            Ref<FrameBuffer> buf = FrameBuffer::Create({kSkyboxSize, kSkyboxSize});
            buf->Bind();
            buf->SetDepthAttachment(Texture::Format::F32);
            buf->AddColorAttachment(res, i);
            Renderer::SetViewport(kSkyboxSize, kSkyboxSize);
            Renderer::Clear();
            tex->Bind(1);
            Renderer::Submit(cube, shader);
        }
        return res;
    }
    void SkyBox::BakeIBL() {
        irradiance_             = CubeMap::Create(kSkyboxSize, Texture::Format::U8, Texture::Channels::RGB);
        Ref<Shader>      shader = Renderer::GetShader(Renderer::Shaders::Convolution);
        Ref<VertexArray> cube   = Renderer::GetVertexArray(Renderer::VertexArrays::Cube);

        Mat4 projection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
        Mat4 views[]    = {glm::lookAt(Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f), Vec3(0.0f, -1.0f, 0.0f)),
                           glm::lookAt(Vec3(0.0f, 0.0f, 0.0f), Vec3(-1.0f, 0.0f, 0.0f), Vec3(0.0f, -1.0f, 0.0f)),
                           glm::lookAt(Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f), Vec3(0.0f, 0.0f, 1.0f)),
                           glm::lookAt(Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, -1.0f, 0.0f), Vec3(0.0f, 0.0f, -1.0f)),
                           glm::lookAt(Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 1.0f), Vec3(0.0f, -1.0f, 0.0f)),
                           glm::lookAt(Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, -1.0f), Vec3(0.0f, -1.0f, 0.0f))};
        shader->Bind();
        shader->SetInt("u_CubeMap", 1);
        shader->SetMat4("u_Transform", Mat4(1.0));
        shader->SetMat4("u_Projection", projection);
        for (U32 i = 0; i < 6; ++i) {
            shader->SetMat4("u_View", views[i]);
            Ref<FrameBuffer> buf = FrameBuffer::Create({kSkyboxSize, kSkyboxSize});
            buf->Bind();
            buf->SetDepthAttachment(Texture::Format::F32);
            buf->AddColorAttachment(irradiance_, i);
            Renderer::SetViewport(kSkyboxSize, kSkyboxSize);
            Renderer::Clear();
            raw_->Bind(1);
            Renderer::Submit(cube, shader);
        }

        U32 curr_size           = kSkyboxSize;
        prefilter_              = CubeMap::Create(curr_size, Texture::Format::U8, Texture::Channels::RGB, true);
        Ref<Shader> s_prefilter = Renderer::GetShader(Renderer::Shaders::PreFileterConvolution);
        s_prefilter->Bind();
        s_prefilter->SetInt("u_CubeMap", 1);
        s_prefilter->SetMat4("u_Projection", projection);
        s_prefilter->SetMat4("u_Transform", Mat4(1.0));
        for (U32 lod = 0; lod < kMaxLevels; ++lod) {
            float roughness = (float)lod / (kMaxLevels - 1);
            s_prefilter->SetFloat("u_Roughness", roughness);
            for (U32 i = 0; i < 6; ++i) {
                s_prefilter->SetMat4("u_View", views[i]);
                Ref<FrameBuffer> buf = FrameBuffer::Create({curr_size, curr_size});
                buf->Bind();
                buf->SetDepthAttachment(Texture::Format::F32);
                buf->AddColorAttachment(prefilter_, i, lod);
                raw_->Bind(1);
                Renderer::SetViewport(curr_size, curr_size);
                Renderer::Clear();
                Renderer::Submit(cube, s_prefilter);
            }
            curr_size /= 2;
        }
    }

}  // namespace DummyEngine