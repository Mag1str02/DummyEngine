#include "DummyEngine/Core/Rendering/RendererOpenGL/GLTexture.h"

#include "DummyEngine/Core/Rendering/RendererOpenGL/GLFrameBuffer.h"
#include "DummyEngine/Core/Rendering/RendererOpenGL/GLUtils.h"

namespace DE {

    GLTexture::GLTexture(uint32_t width, uint32_t height, Texture::Channels channels, Texture::Format format) :
        m_Width(0), m_Height(0), m_Format(format), m_Channels(channels) {
        glGenTextures(1, &m_TextureId);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_TextureId);
        // TODO: Move tex parameters somewhere else...
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        Resize(width, height);
    }
    GLTexture::GLTexture(Texture::Channels channels, Texture::Format format) : m_Width(0), m_Height(0), m_Format(format), m_Channels(channels) {
        glGenTextures(1, &m_TextureId);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_TextureId);
        // TODO: Move tex parameters somewhere else...
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }
    GLTexture::GLTexture(const TextureResource& data) :
        m_Width(data.Width()),
        m_Height(data.Height()),
        m_Format(Texture::DataFormat(data.Format())),
        m_Channels(Texture::DataChannels(data.Channels())) {
        glGenTextures(1, &m_TextureId);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_TextureId);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        Invalidate(GLDataType(data.Format()), data.Data());
    }

    GLTexture::~GLTexture() {
        Remove();
    }

    U32 GLTexture::GetWidth() const {
        return m_Width;
    }
    U32 GLTexture::GetHeight() const {
        return m_Height;
    }
    U32 GLTexture::GetRendererId() const {
        return m_TextureId;
    }
    Texture::Format GLTexture::GetFormat() const {
        return m_Format;
    }
    Texture::Channels GLTexture::GetChannels() const {
        return m_Channels;
    }

    void GLTexture::Copy(Ref<FrameBuffer> buffer, U32 attachment_id) {
        GLTexture* source = reinterpret_cast<GLTexture*>(buffer->GetColorAttachment(attachment_id).get());

        m_Height   = source->m_Height;
        m_Width    = source->m_Width;
        m_Format   = source->m_Format;
        m_Channels = source->m_Channels;
        Invalidate();

        buffer->Bind();
        glReadBuffer(GL_COLOR_ATTACHMENT0 + attachment_id);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_TextureId);
        glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, m_Width, m_Height);
    }

    void GLTexture::SetFormat(Format format) {
        if (m_Format != format) {
            m_Format = format;
            Invalidate();
        }
    }
    void GLTexture::SetChannels(Channels channels) {
        if (m_Channels != channels) {
            m_Channels = channels;
            Invalidate();
        }
    }
    void GLTexture::Resize(U32 width, U32 height) {
        DE_ASSERT(width * height > 0, "Cannot resize texture to size 0");
        if (m_Width != width || m_Height != height) {
            m_Width  = width;
            m_Height = height;
            Invalidate();
        }
    }
    void GLTexture::Bind(U32 unit_id) const {
        DE_ASSERT(unit_id > 0, "Cannot bind to texture slot 0, because it's reserved by engine");
        glActiveTexture(GL_TEXTURE0 + unit_id);
        glBindTexture(GL_TEXTURE_2D, m_TextureId);
    }
    void GLTexture::Invalidate(GLenum data_type, const void* data) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_TextureId);
        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     GLTextureFormatInternal(m_Format, m_Channels),
                     m_Width,
                     m_Height,
                     0,
                     GLTextureFormatExternal(m_Channels),
                     data_type,
                     data);
        // glGenerateMipmap(GL_TEXTURE_2D);
    }
    void GLTexture::Remove() {
        glDeleteTextures(1, &m_TextureId);
    }
}  // namespace DE
