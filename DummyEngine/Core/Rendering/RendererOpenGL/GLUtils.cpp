#include "DummyEngine/Core/Rendering/RendererOpenGL/GLUtils.h"

#include "DummyEngine/Core/Rendering/Renderer/RenderAPI.h"

namespace DE {
    GLenum TextureFormatToGLTextureFormat(TextureFormat format) {
        switch (format) {
            case TextureFormat::None: return GL_RED;
            case TextureFormat::RED: return GL_RED;
            case TextureFormat::RA: return GL_RG;
            case TextureFormat::RGB: return GL_RGB;
            case TextureFormat::RGBA: return GL_RGBA;
            case TextureFormat::Depth: return GL_DEPTH_COMPONENT;
            case TextureFormat::None: DE_ASSERT(false, "TextureFormat not specified"); break;
            default: DE_ASSERT(false, "Unsupported TextureFormat");
        }
        return GL_RED;
    }
    GLenum TextureFormatToGLTextureInternalFormat(TextureFormat format) {
        switch (format) {
            case TextureFormat::RED: return GL_R8;
            case TextureFormat::RA: return GL_RG8;
            case TextureFormat::RGB: return GL_RGB8;
            case TextureFormat::RGBA: return GL_RGBA8;
            case TextureFormat::Depth: return GL_DEPTH_COMPONENT32F;
            case TextureFormat::None: DE_ASSERT(false, "TextureFormat not specified"); break;
            default: DE_ASSERT(false, "Unsupported TextureFormat");
        }
        return GL_R8;
    }

    GLenum RenderPrimitiveToGL(RenderPrimitive primitive) {
        switch (primitive) {
            case RenderPrimitive::Triangle: return GL_TRIANGLES;
            case RenderPrimitive::Point: return GL_POINTS;
            case RenderPrimitive::None: DE_ASSERT(false, "TexturePrimitive not specified"); break;
            default: DE_ASSERT(false, "Unsupported RenderPrimitive");
        }
        return GL_R8;
    }
}  // namespace DE