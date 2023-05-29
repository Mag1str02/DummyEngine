#include "DummyEngine/Core/Rendering/Renderer/SkyBox.h"

#include <glad/glad.h>

#include "DummyEngine/Core/Rendering/Renderer/FrameBuffer.h"
#include "DummyEngine/Core/Rendering/Renderer/Renderer.h"

namespace DE {
    const U32 SKYBOX_SIZE             = 2048;
    const U32 IRRADIANCE_TEXTURE_UNIT = 6;
    const U32 PREFILTER_TEXTURE_UNIT  = 7;
    const U32 BRDF_TEXTURE_UNIT       = 8;
    const U32 MAX_LEVELS              = 10;

    SkyBox::SkyBox(Ref<TextureData> texture) {
        m_Raw = GenRawCubeMap(texture);
        BakeIBL();
    }
    SkyBox::SkyBox(Ref<CubeMap> cubemap) {
        m_Raw = cubemap;
        BakeIBL();
    }

    Ref<CubeMap> SkyBox::GetMap() {
        return m_Raw;
    }
    void SkyBox::ApplyIBL(Ref<Shader> shader) {
        if (!m_Irradiance) {
            return;
        }
        if (!m_Prefilter) {
            return;
        }
        m_Irradiance->Bind(IRRADIANCE_TEXTURE_UNIT);
        m_Prefilter->Bind(PREFILTER_TEXTURE_UNIT);
        Renderer::GetTexture(Renderer::Textures::BRDF)->Bind(BRDF_TEXTURE_UNIT);
        shader->SetInt("u_IrradianceMap", IRRADIANCE_TEXTURE_UNIT);
        shader->SetInt("u_PrefilterMap", PREFILTER_TEXTURE_UNIT);
        shader->SetInt("u_BRDF", BRDF_TEXTURE_UNIT);
    }

    Ref<CubeMap> SkyBox::GenRawCubeMap(Ref<TextureData> texture) {
        Ref<Texture>     tex    = Texture::Create(*texture);
        Ref<CubeMap>     res    = CubeMap::Create(SKYBOX_SIZE, texture->Format(), texture->Channels());
        Ref<Shader>      shader = Renderer::GetShader(Renderer::Shaders::EquirectangularToCubeMap);
        Ref<VertexArray> cube   = Renderer::GetVertexArray(Renderer::VertexArrays::Cube);

        Mat4 projection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
        Mat4 views[]    = {glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
                           glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
                           glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
                           glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)),
                           glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
                           glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f))};

        shader->Bind();
        shader->SetInt("u_EquirectangularMap", 1);
        shader->SetMat4("u_Projection", projection);
        for (U32 i = 0; i < 6; ++i) {
            shader->SetMat4("u_View", views[i]);
            Ref<FrameBuffer> buf = FrameBuffer::Create({SKYBOX_SIZE, SKYBOX_SIZE});
            buf->Bind();
            buf->SetDepthAttachment(TextureChannels::Depth);
            buf->AddColorAttachment(res, i);
            Renderer::SetViewport(SKYBOX_SIZE, SKYBOX_SIZE);
            Renderer::Clear();
            tex->Bind(1);
            Renderer::Submit(cube, shader);
        }
        return res;
    }
    void SkyBox::BakeIBL() {
        m_Irradiance            = CubeMap::Create(SKYBOX_SIZE, TextureFormat::U8, TextureChannels::RGB);
        Ref<Shader>      shader = Renderer::GetShader(Renderer::Shaders::Convolution);
        Ref<VertexArray> cube   = Renderer::GetVertexArray(Renderer::VertexArrays::Cube);

        Mat4 projection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
        Mat4 views[]    = {glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
                           glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
                           glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
                           glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)),
                           glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
                           glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f))};
        shader->Bind();
        shader->SetInt("u_CubeMap", 1);
        shader->SetMat4("u_Transform", Mat4(1.0));
        shader->SetMat4("u_Projection", projection);
        for (U32 i = 0; i < 6; ++i) {
            shader->SetMat4("u_View", views[i]);
            Ref<FrameBuffer> buf = FrameBuffer::Create({SKYBOX_SIZE, SKYBOX_SIZE});
            buf->Bind();
            buf->SetDepthAttachment(TextureChannels::Depth);
            buf->AddColorAttachment(m_Irradiance, i);
            Renderer::SetViewport(SKYBOX_SIZE, SKYBOX_SIZE);
            Renderer::Clear();
            m_Raw->Bind(1);
            Renderer::Submit(cube, shader);
        }

        U32 curr_size           = SKYBOX_SIZE;
        m_Prefilter             = CubeMap::Create(curr_size, TextureFormat::U8, TextureChannels::RGB, true);
        Ref<Shader> s_prefilter = Renderer::GetShader(Renderer::Shaders::PreFileterConvolution);
        s_prefilter->Bind();
        s_prefilter->SetInt("u_CubeMap", 1);
        s_prefilter->SetMat4("u_Projection", projection);
        s_prefilter->SetMat4("u_Transform", Mat4(1.0));
        for (U32 lod = 0; lod < MAX_LEVELS; ++lod) {
            float roughness = (float)lod / (MAX_LEVELS - 1);
            s_prefilter->SetFloat("u_Roughness", roughness);
            for (U32 i = 0; i < 6; ++i) {
                s_prefilter->SetMat4("u_View", views[i]);
                Ref<FrameBuffer> buf = FrameBuffer::Create({curr_size, curr_size});
                buf->Bind();
                buf->SetDepthAttachment(TextureChannels::Depth);
                buf->AddColorAttachment(m_Prefilter, i, lod);
                m_Raw->Bind(1);
                Renderer::SetViewport(curr_size, curr_size);
                Renderer::Clear();
                Renderer::Submit(cube, s_prefilter);
            }
            curr_size /= 2;
        }
    }
}  // namespace DE