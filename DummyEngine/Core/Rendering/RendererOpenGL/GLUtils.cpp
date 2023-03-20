#include "DummyEngine/Core/Rendering/RendererOpenGL/GLUtils.h"

#include "DummyEngine/Core/Rendering/Renderer/RenderAPI.h"

namespace DE {
    GLenum TextureFormatToGLTextureFormat(TextureFormat format) {
        switch (format) {
            case TextureFormat::RED: return GL_RED;
            case TextureFormat::RGB: return GL_RGB;
            case TextureFormat::RGBA: return GL_RGBA;
            case TextureFormat::None: return GL_RED;
            default: return GL_RED;
        }
    }
    GLenum TextureFormatToGLTextureInternalFormat(TextureFormat format) {
        switch (format) {
            case TextureFormat::RED: return GL_R8;
            case TextureFormat::RGB: return GL_RGB8;
            case TextureFormat::RGBA: return GL_RGBA8;
            case TextureFormat::DepthStencil: return GL_DEPTH24_STENCIL8;
            default: return GL_R8;
        }
    }

    GLenum RenderPrimitiveToGL(RenderPrimitive primitive) {
        switch (primitive) {
            case RenderPrimitive::Triangle: return GL_TRIANGLES;
            case RenderPrimitive::Point: return GL_POINTS;
            default: return GL_TRIANGLES;
        }
    }
}  // namespace DE