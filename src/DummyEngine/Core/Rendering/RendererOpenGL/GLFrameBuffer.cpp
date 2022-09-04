#include "Core/Rendering/RendererOpenGL/GLFrameBuffer.h"
#include "Core/Rendering/RendererOpenGL/GLTexture.h"

namespace DE
{
    bool IsColorFormat(TextureFormat format)
    {
        switch (format)
        {
            case TextureFormat::RED: return true;
            case TextureFormat::RGB: return true;
            case TextureFormat::RGBA: return true;
        }
        return false;
    }
    bool IsDepthFormat(TextureFormat format)
    {
        switch (format)
        {
            case TextureFormat::DepthStencil: return true;
        }
        return false;
    }

    void GLFrameBuffer::Rebuild()
    {
        DE_PROFILE_SCOPE("FrameBuffer Rebuild");

        if (m_BufferId)
        {
            glDeleteFramebuffers(1, &m_BufferId);
            m_DepthAttachment.m_Texture = nullptr;
            for (auto& attachment : m_ColorAttachments)
            {
                attachment.m_Texture = nullptr;
            }
        }

        glCreateFramebuffers(1, &m_BufferId);
        glBindFramebuffer(GL_FRAMEBUFFER, m_BufferId);

        {
            for (size_t i = 0; i < m_ColorAttachments.size(); ++i)
            {
                auto& attachment = m_ColorAttachments[i];
                DE_ASSERT(IsColorFormat(attachment.m_Format), "Worng texture format for FrameBuffer color attachment.");
                attachment.m_Texture = Texture::Create(m_Properties.width, m_Properties.height, attachment.m_Format);

                // TODO: Try to avoid this
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, std::dynamic_pointer_cast<GLTexture>(attachment.m_Texture)->m_TextureId, 0);
            }
            DE_ASSERT(IsDepthFormat(m_DepthAttachment.m_Format), "Worng texture format for FrameBuffer depth attachment.");
            m_DepthAttachment.m_Texture = Texture::Create(m_Properties.width, m_Properties.height, m_DepthAttachment.m_Format);

            // TODO: and this...
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, std::dynamic_pointer_cast<GLTexture>(m_DepthAttachment.m_Texture)->m_TextureId, 0);
        }
    }

    GLFrameBuffer::GLFrameBuffer(const FrameBufferProperties& properties) : m_BufferId(0), m_Properties(properties) { glCreateFramebuffers(1, &m_BufferId); }
    GLFrameBuffer::~GLFrameBuffer() { glDeleteBuffers(1, &m_BufferId); }

    void GLFrameBuffer::Bind()
    {
        DE_PROFILE_SCOPE("FrameBuffer Bind");

        glBindFramebuffer(GL_FRAMEBUFFER, m_BufferId);
    }
    void GLFrameBuffer::UnBind() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

    void GLFrameBuffer::AddColorAttachment(TextureFormat format)
    {
        // TODO: Switch 8 to GPU capabilities.
        DE_ASSERT(m_ColorAttachments.size() <= 8, "No available color attachment slot.");
        glBindFramebuffer(GL_FRAMEBUFFER, m_BufferId);
        m_ColorAttachments.push_back({nullptr, format});
        m_ColorAttachments.back().m_Texture = Texture::Create(m_Properties.width, m_Properties.height, format);
        glFramebufferTexture2D(
            GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + m_ColorAttachments.size() - 1, GL_TEXTURE_2D, std::dynamic_pointer_cast<GLTexture>(m_ColorAttachments.back().m_Texture)->m_TextureId, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    void GLFrameBuffer::SetDepthAttachment(TextureFormat format)
    {
        // TODO: Switch 8 to GPU capabilities.
        glBindFramebuffer(GL_FRAMEBUFFER, m_BufferId);
        m_DepthAttachment.m_Format = format;
        m_DepthAttachment.m_Texture = Texture::Create(m_Properties.width, m_Properties.height, format);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, std::dynamic_pointer_cast<GLTexture>(m_DepthAttachment.m_Texture)->m_TextureId, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    void GLFrameBuffer::Resize(uint32_t width, uint32_t height)
    {
        DE_PROFILE_SCOPE("FrameBuffer Resize");

        if ((m_Properties.width != width || m_Properties.height != height) && 0 < width && 0 < height)
        {
            m_Properties.width = width;
            m_Properties.height = height;
            Rebuild();
        }
    }

    Ref<Texture> GLFrameBuffer::GetColorAttachment(uint32_t attachment_id = 0)
    {
        DE_ASSERT(0 <= attachment_id && attachment_id < m_ColorAttachments.size(), "Color attachment with index: " + std::to_string(attachment_id) + " does not exist.");
        return m_ColorAttachments[attachment_id].m_Texture;
    }
    Ref<Texture> GLFrameBuffer::GetDepthAttachment() { return m_DepthAttachment.m_Texture; }

    const FrameBufferProperties& GLFrameBuffer::GetProperties() const { return m_Properties; }

}  // namespace DE