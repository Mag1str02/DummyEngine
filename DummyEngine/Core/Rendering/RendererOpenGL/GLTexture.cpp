#include "DummyEngine/Core/Rendering/RendererOpenGL/GLTexture.h"

#include "DummyEngine/Core/Rendering/RendererOpenGL/GLUtils.h"

namespace DE {

    GLTexture::GLTexture(uint32_t width, uint32_t height, Texture::Channels channels, Texture::Format format) :
        m_Width(0), m_Height(0), m_InternalFormat(GLTextureFormatInternal(format, channels)), m_Format(GLTextureFormatExternal(channels)) {
        glGenTextures(1, &m_TextureId);
        glBindTexture(GL_TEXTURE_2D, m_TextureId);
        // TODO: Move tex parameters somewhere else...
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        Resize(width, height);
    }
    GLTexture::GLTexture(Texture::Channels channels, Texture::Format format) :
        m_Width(0), m_Height(0), m_InternalFormat(GLTextureFormatInternal(format, channels)), m_Format(GLTextureFormatExternal(channels)) {
        glGenTextures(1, &m_TextureId);
        glBindTexture(GL_TEXTURE_2D, m_TextureId);
        // TODO: Move tex parameters somewhere else...
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }
    GLTexture::GLTexture(const TextureData& data) :
        m_Width(data.Width()),
        m_Height(data.Height()),
        m_InternalFormat(GLTextureFormatInternal(Texture::DataFormat(data.Format()), Texture::DataChannels(data.Channels()))),
        m_Format(GLTextureFormatExternal(Texture::DataChannels(data.Channels()))) {
        glGenTextures(1, &m_TextureId);
        glBindTexture(GL_TEXTURE_2D, m_TextureId);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, m_Width, m_Height, 0, m_Format, GLDataType(data.Format()), data.Data());

        glGenerateMipmap(GL_TEXTURE_2D);
    }

    GLTexture::~GLTexture() {
        glDeleteTextures(1, &m_TextureId);
    }

    U32 GLTexture::Width() const {
        return m_Width;
    }
    U32 GLTexture::Height() const {
        return m_Height;
    }
    U32 GLTexture::RendererId() const {
        return m_TextureId;
    }
    void GLTexture::Resize(U32 width, U32 height) {
        DE_ASSERT(width * height > 0, "Cannot resize texture to size 0");
        m_Width  = width;
        m_Height = height;
        glBindTexture(GL_TEXTURE_2D, m_TextureId);
        glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, m_Width, m_Height, 0, m_Format, GL_UNSIGNED_BYTE, nullptr);
    }
    void GLTexture::Bind(U32 unit_id) const {
        glActiveTexture(GL_TEXTURE0 + unit_id);
        glBindTexture(GL_TEXTURE_2D, m_TextureId);
    }
}  // namespace DE
