#include "DummyEngine/Core/Rendering/RendererOpenGL/GLTexture.h"

#include "DummyEngine/Core/Rendering/RendererOpenGL/GLUtils.h"

namespace DE {

    GLTexture::GLTexture(uint32_t width, uint32_t height, TextureFormat format) :
        m_Width(width),
        m_Height(height),
        m_InternalFormat(TextureFormatToGLTextureInternalFormat(format)),
        m_Format(TextureFormatToGLTextureFormat(format)) {
        glGenTextures(1, &m_TextureId);
        glBindTexture(GL_TEXTURE_2D, m_TextureId);
        glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, m_Width, m_Height, 0, m_Format, GL_UNSIGNED_BYTE, nullptr);
        // TODO: Move tex parameters somewhere else...
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glGenerateMipmap(GL_TEXTURE_2D);
    }
    GLTexture::GLTexture(const TextureData& data) :
        m_Width(data.Width()),
        m_Height(data.Height()),
        m_InternalFormat(TextureFormatToGLTextureInternalFormat(data.Format())),
        m_Format(TextureFormatToGLTextureFormat(data.Format())) {
        glGenTextures(1, &m_TextureId);
        glBindTexture(GL_TEXTURE_2D, m_TextureId);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, m_Width, m_Height, 0, m_Format, GL_UNSIGNED_BYTE, data.Data());

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
    void GLTexture::SetData(const void* data, uint32_t size) {
        DE_ASSERT(size == m_Width * m_Height, "Data size mismatches texture size (", size, ") expected (", m_Width * m_Height, ")");
        glBindTexture(GL_TEXTURE_2D, m_TextureId);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_Width, m_Height, m_Format, GL_UNSIGNED_BYTE, data);
    }

    void GLTexture::Bind(U32 unit_id) const {
        glActiveTexture(GL_TEXTURE0 + unit_id);
        glBindTexture(GL_TEXTURE_2D, m_TextureId);
    }
}  // namespace DE
