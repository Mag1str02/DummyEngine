#include "DummyEngine/Core/Rendering/RendererOpenGL/GLTexture.h"

#include "DummyEngine/Core/Rendering/RendererOpenGL/GLUtils.h"

namespace DE {

    GLTexture::GLTexture(U32 width, U32 height, TextureFormat format) :
        m_Width(width),
        m_Height(height),
        m_InternalFormat(TextureFormatToGLTextureInternalFormat(format)),
        m_Format(TextureFormatToGLTextureFormat(format)) {
        glCreateTextures(GL_TEXTURE_2D, 1, &m_TextureId);
        glTextureStorage2D(m_TextureId, 1, m_InternalFormat, m_Width, m_Height);

        // TODO: Move tex parameters somewhere else...
        glTextureParameteri(m_TextureId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(m_TextureId, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTextureParameteri(m_TextureId, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(m_TextureId, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }
    GLTexture::GLTexture(const TextureData& data) :
        m_Width(data.Width()),
        m_Height(data.Height()),
        m_InternalFormat(TextureFormatToGLTextureInternalFormat(data.Format())),
        m_Format(TextureFormatToGLTextureFormat(data.Format())) {
        glCreateTextures(GL_TEXTURE_2D, 1, &m_TextureId);

        glTextureParameteri(m_TextureId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(m_TextureId, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTextureParameteri(m_TextureId, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(m_TextureId, GL_TEXTURE_WRAP_T, GL_REPEAT);

        if (data.Data() != nullptr) {
            glTextureStorage2D(m_TextureId, 1, m_InternalFormat, m_Width, m_Height);
            glTextureSubImage2D(m_TextureId, 0, 0, 0, m_Width, m_Height, m_Format, GL_UNSIGNED_BYTE, data.Data());
        }
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

    void GLTexture::SetData(const void* data, U32 size) {
        DE_ASSERT(size == m_Width * m_Height, "Data size mismatches texture size (", size, ") expected (", m_Width * m_Height, ")");
        glTextureSubImage2D(m_TextureId, 0, 0, 0, m_Width, m_Height, m_Format, GL_UNSIGNED_BYTE, data);
    }

    void GLTexture::Bind(U32 unit_id) const {
        glActiveTexture(GL_TEXTURE0 + unit_id);
        glBindTexture(GL_TEXTURE_2D, m_TextureId);
    }
}  // namespace DE
