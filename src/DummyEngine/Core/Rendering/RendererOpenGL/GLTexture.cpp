#include "DummyEngine/Core/Rendering/RendererOpenGL/GLTexture.h"
#include "DummyEngine/ToolBox/Dev/Logger.h"

namespace DE
{

    GLenum TextureFormatToGLTextureFormat(TextureFormat format)
    {
        switch (format)
        {
            case TextureFormat::RED: return GL_RED;
            case TextureFormat::RGB: return GL_RGB;
            case TextureFormat::RGBA: return GL_RGBA;
            case TextureFormat::None: return GL_RED;
        }
        return GL_RED;
    }
    GLenum TextureFormatToGLTextureInternalFormat(TextureFormat format)
    {
        switch (format)
        {
            case TextureFormat::RED: return GL_R8;
            case TextureFormat::RGB: return GL_RGB8;
            case TextureFormat::RGBA: return GL_RGBA8;
            case TextureFormat::None: return GL_R8;
        }
        return GL_R8;
    }

    GLTexture::GLTexture(uint32_t width, uint32_t height, TextureFormat format) :
        m_Width(width),
        m_Height(height),
        m_InternalFormat(TextureFormatToGLTextureInternalFormat(format)),
        m_Format(TextureFormatToGLTextureFormat(format))
    {
        glCreateTextures(GL_TEXTURE_2D, 1, &m_TextureId);
        glTextureStorage2D(m_TextureId, 1, m_InternalFormat, m_Width, m_Height);

        glTextureParameteri(m_TextureId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(m_TextureId, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTextureParameteri(m_TextureId, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(m_TextureId, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }
    GLTexture::GLTexture(const TextureData& data) :
        m_Width(data.width),
        m_Height(data.height),
        m_InternalFormat(TextureFormatToGLTextureInternalFormat(data.format)),
        m_Format(TextureFormatToGLTextureFormat(data.format))
    {
        glCreateTextures(GL_TEXTURE_2D, 1, &m_TextureId);

        glTextureParameteri(m_TextureId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(m_TextureId, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTextureParameteri(m_TextureId, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(m_TextureId, GL_TEXTURE_WRAP_T, GL_REPEAT);

        if (data.data != nullptr)
        {
            glTextureStorage2D(m_TextureId, 1, m_InternalFormat, m_Width, m_Height);
            glTextureSubImage2D(m_TextureId, 0, 0, 0, m_Width, m_Height, m_Format, GL_UNSIGNED_BYTE, *data.data);
        }
    }

    GLTexture::~GLTexture()
    {
        glDeleteTextures(1, &m_TextureId);
    }

    uint32_t GLTexture::Width() const
    {
        return m_Width;
    }
    uint32_t GLTexture::Height() const
    {
        return m_Height;
    }

    void GLTexture::SetData(const void* data, uint32_t size)
    {
        DE_ASSERT(size == m_Width * m_Height, "Data size mismatches texture size.");
        glTextureSubImage2D(m_TextureId, 0, 0, 0, m_Width, m_Height, m_Format, GL_UNSIGNED_BYTE, data);
    }

    void GLTexture::Bind(uint32_t unit_id) const
    {
        glActiveTexture(GL_TEXTURE0 + unit_id);
        glBindTexture(GL_TEXTURE_2D, m_TextureId);
    }
}  // namespace DE
