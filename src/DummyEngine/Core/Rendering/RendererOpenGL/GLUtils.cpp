#include "Core/Rendering/RendererOpenGL/GLUtils.h"

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
            case TextureFormat::DepthStencil: return GL_DEPTH24_STENCIL8;
        }
        return GL_R8;
    }
}  // namespace DE