#include "DummyEngine/Core/Rendering/RendererOpenGL/GLFrameBuffer.h"

#include "DummyEngine/Core/Rendering/RendererOpenGL/GLCubeMap.h"
#include "DummyEngine/Core/Rendering/RendererOpenGL/GLTexture.h"

namespace DE {
    bool IsColorFormat(Texture::Channels channels) {
        switch (channels) {
            case Texture::Channels::R: return true;
            case Texture::Channels::RG: return true;
            case Texture::Channels::RGB: return true;
            case Texture::Channels::RGBA: return true;
            default: return false;
        }
    }
    bool IsDepthFormat(Texture::Channels channels) {
        switch (channels) {
            case Texture::Channels::Depth: return true;
            default: return false;
        }
    }

    void GLFrameBuffer::Rebuild() {
        for (size_t i = 0; i < m_ColorAttachments.size(); ++i) {
            auto& attachment = m_ColorAttachments[i];
            attachment.m_Texture->Resize(m_Properties.width, m_Properties.height);
        }
        m_DepthAttachment.m_Texture->Resize(m_Properties.width, m_Properties.height);
    }

    GLFrameBuffer::GLFrameBuffer(const FrameBufferProperties& properties) : m_BufferId(0), m_Properties(properties) {
        glGenFramebuffers(1, &m_BufferId);
    }
    GLFrameBuffer::~GLFrameBuffer() {
        glDeleteFramebuffers(1, &m_BufferId);
    }

    void GLFrameBuffer::Bind() {
        glBindFramebuffer(GL_FRAMEBUFFER, m_BufferId);
    }
    void GLFrameBuffer::UnBind() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void GLFrameBuffer::AddColorAttachment(Texture::Format format, Texture::Channels channels) {
        // TODO: Switch 8 to GPU capabilities.
        DE_ASSERT(IsColorFormat(channels), "Wrong channels for color attachment");
        DE_ASSERT(m_ColorAttachments.size() <= 8, "No available color attachment slot");
        glBindFramebuffer(GL_FRAMEBUFFER, m_BufferId);
        m_ColorAttachments.push_back({nullptr});
        m_ColorAttachments.back().m_Texture = Texture::Create(m_Properties.width, m_Properties.height, channels, format);
        glFramebufferTexture2D(GL_FRAMEBUFFER,
                               GL_COLOR_ATTACHMENT0 + m_ColorAttachments.size() - 1,
                               GL_TEXTURE_2D,
                               std::dynamic_pointer_cast<GLTexture>(m_ColorAttachments.back().m_Texture)->m_TextureId,
                               0);
    }
    void GLFrameBuffer::AddColorAttachment(Ref<CubeMap> map, U32 side, U32 lod) {
        // TODO: Rework
        glBindFramebuffer(GL_FRAMEBUFFER, m_BufferId);
        m_ColorAttachments.push_back({nullptr});
        glFramebufferTexture2D(GL_FRAMEBUFFER,
                               GL_COLOR_ATTACHMENT0 + m_ColorAttachments.size() - 1,
                               GL_TEXTURE_CUBE_MAP_POSITIVE_X + side,
                               std::dynamic_pointer_cast<GLCubeMap>(map)->m_MapId,
                               lod);
    }
    void GLFrameBuffer::SetDepthAttachment(Texture::Format format) {
        glBindFramebuffer(GL_FRAMEBUFFER, m_BufferId);
        m_DepthAttachment.m_Texture = Texture::Create(m_Properties.width, m_Properties.height, Texture::Channels::Depth, format, true);
        glFramebufferTexture2D(
            GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, std::dynamic_pointer_cast<GLTexture>(m_DepthAttachment.m_Texture)->m_TextureId, 0);
    }
    void GLFrameBuffer::SetColorAttachment(Ref<Texture> texture, U32 id) {
        if (m_ColorAttachments.size() <= id) {
            m_ColorAttachments.resize(id + 1);
        }
        m_ColorAttachments[id].m_Texture = texture;
        glBindFramebuffer(GL_FRAMEBUFFER, m_BufferId);
        glFramebufferTexture2D(
            GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + id, GL_TEXTURE_2D, std::dynamic_pointer_cast<GLTexture>(texture)->m_TextureId, 0);
    }
    void GLFrameBuffer::SetDepthAttachment(Ref<CubeMap> map) {
        glBindFramebuffer(GL_FRAMEBUFFER, m_BufferId);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, std::dynamic_pointer_cast<GLCubeMap>(map)->m_MapId, 0);
    }
    bool GLFrameBuffer::Valid() const {
        glBindFramebuffer(GL_FRAMEBUFFER, m_BufferId);
        GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        return status == GL_FRAMEBUFFER_COMPLETE;
    }
    void GLFrameBuffer::Resize(U32 width, U32 height) {
        if ((m_Properties.width != width || m_Properties.height != height) && 0 < width && 0 < height) {
            m_Properties.width  = width;
            m_Properties.height = height;
            Rebuild();
        }
    }
    U32 GLFrameBuffer::GetWidth() const {
        return m_Properties.width;
    }
    U32 GLFrameBuffer::GetHeight() const {
        return m_Properties.height;
    }

    Ref<Texture> GLFrameBuffer::GetColorAttachment(U32 attachment_id = 0) {
        DE_ASSERT(0 <= attachment_id && attachment_id < m_ColorAttachments.size(), "Color attachment with index {} does not exist", attachment_id);
        return m_ColorAttachments[attachment_id].m_Texture;
    }
    Ref<Texture> GLFrameBuffer::GetDepthAttachment() {
        return m_DepthAttachment.m_Texture;
    }

    const FrameBufferProperties& GLFrameBuffer::GetProperties() const {
        return m_Properties;
    }

}  // namespace DE