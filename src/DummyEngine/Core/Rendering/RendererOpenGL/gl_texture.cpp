#include "DummyEngine/Core/Rendering/RendererOpenGL/gl_texture.h"
#include "DummyEngine/ToolBox/Dev/logger.h"

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
    }

    GLTexture::GLTexture(uint32_t width, uint32_t height, TextureFormat format) :
        _width(width),
        _height(height),
        _internal_format(TextureFormatToGLTextureInternalFormat(format)),
        _format(TextureFormatToGLTextureFormat(format))
    {
        glCreateTextures(GL_TEXTURE_2D, 1, &_texture_id);
        glTextureStorage2D(_texture_id, 1, _internal_format, _width, _height);

        glTextureParameteri(_texture_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(_texture_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTextureParameteri(_texture_id, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(_texture_id, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }
    GLTexture::GLTexture(const TextureData& data) :
        _width(data.width),
        _height(data.height),
        _internal_format(TextureFormatToGLTextureInternalFormat(data.format)),
        _format(TextureFormatToGLTextureFormat(data.format))
    {
        glCreateTextures(GL_TEXTURE_2D, 1, &_texture_id);

        glTextureParameteri(_texture_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(_texture_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTextureParameteri(_texture_id, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(_texture_id, GL_TEXTURE_WRAP_T, GL_REPEAT);

        if (data.data != nullptr)
        {
            glTextureStorage2D(_texture_id, 1, _internal_format, _width, _height);
            glTextureSubImage2D(_texture_id, 0, 0, 0, _width, _height, _format, GL_UNSIGNED_BYTE, *data.data);
        }
    }

    GLTexture::~GLTexture()
    {
        glDeleteTextures(1, &_texture_id);
    }

    uint32_t GLTexture::Width() const
    {
        return _width;
    }
    uint32_t GLTexture::Height() const
    {
        return _height;
    }

    void GLTexture::SetData(const void* data)
    {
        glTextureSubImage2D(_texture_id, 0, 0, 0, _width, _height, _format, GL_UNSIGNED_BYTE, data);
    }

    void GLTexture::Bind(uint32_t unit_id) const
    {
        glActiveTexture(GL_TEXTURE0 + unit_id);
        glBindTexture(GL_TEXTURE_2D, _texture_id);
    }
}  // namespace DE
