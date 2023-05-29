#include "DummyEngine/Core/Rendering/RendererOpenGL/GLUtils.h"

#include "DummyEngine/Core/Rendering/Renderer/RenderAPI.h"

namespace DE {
    GLenum TextureFormatToGLTextureFormat(TextureChannels format) {
        switch (format) {
            case TextureChannels::RED: return GL_RED;
            case TextureChannels::RG: return GL_RG;
            case TextureChannels::RGB: return GL_RGB;
            case TextureChannels::RGBA: return GL_RGBA;
            case TextureChannels::Depth: return GL_DEPTH_COMPONENT;
            case TextureChannels::None: DE_ASSERT(false, "TextureFormat not specified"); break;
            default: DE_ASSERT(false, "Unsupported TextureFormat");
        }
        return GL_RED;
    }
    GLenum TextureFormatToGLTextureInternalFormat(TextureChannels format) {
        switch (format) {
            case TextureChannels::RED: return GL_R8;
            case TextureChannels::RG: return GL_RG8;
            case TextureChannels::RGB: return GL_RGB8;
            case TextureChannels::RGBA: return GL_RGBA8;
            case TextureChannels::Depth: return GL_DEPTH_COMPONENT32F;
            case TextureChannels::None: DE_ASSERT(false, "TextureFormat not specified"); break;
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