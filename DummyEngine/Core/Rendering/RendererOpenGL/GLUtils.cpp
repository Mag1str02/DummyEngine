#include "DummyEngine/Core/Rendering/RendererOpenGL/GLUtils.h"

#include "DummyEngine/Core/Rendering/Renderer/RenderAPI.h"

namespace DE {
    GLenum GLTextureFormatInternal(Texture::Format format, Texture::Channels channels) {
        if (format == Texture::Format::U8 && channels == Texture::Channels::R) return GL_R8;
        if (format == Texture::Format::U8 && channels == Texture::Channels::RG) return GL_RG8;
        if (format == Texture::Format::U8 && channels == Texture::Channels::RGB) return GL_RGB8;
        if (format == Texture::Format::U8 && channels == Texture::Channels::RGBA) return GL_RGBA8;

        if (format == Texture::Format::U16 && channels == Texture::Channels::R) return GL_R16;
        if (format == Texture::Format::U16 && channels == Texture::Channels::RG) return GL_RG16;
        if (format == Texture::Format::U16 && channels == Texture::Channels::RGB) return GL_RGB16_SNORM;
        if (format == Texture::Format::U16 && channels == Texture::Channels::RGBA) return GL_RGBA16;
        if (format == Texture::Format::U16 && channels == Texture::Channels::Depth) return GL_DEPTH_COMPONENT16;

        if (format == Texture::Format::F16 && channels == Texture::Channels::R) return GL_R16F;
        if (format == Texture::Format::F16 && channels == Texture::Channels::RG) return GL_RG16F;
        if (format == Texture::Format::F16 && channels == Texture::Channels::RGB) return GL_RGB16F;
        if (format == Texture::Format::F16 && channels == Texture::Channels::RGBA) return GL_RGBA16F;

        if (format == Texture::Format::F32 && channels == Texture::Channels::R) return GL_R32F;
        if (format == Texture::Format::F32 && channels == Texture::Channels::RG) return GL_RG32F;
        if (format == Texture::Format::F32 && channels == Texture::Channels::RGB) return GL_RGB32F;
        if (format == Texture::Format::F32 && channels == Texture::Channels::RGBA) return GL_RGBA32F;
        if (format == Texture::Format::F32 && channels == Texture::Channels::Depth) return GL_DEPTH_COMPONENT32F;

        DE_ASSERT(false, "Unsupported format and channels combination (", Texture::FormatToStr(format), ", ", Texture::ChannelsToStr(channels), ")");
        return 0;
    }
    GLenum GLTextureFormatExternal(Texture::Channels channels) {
        switch (channels) {
            case Texture::Channels::R: return GL_RED;
            case Texture::Channels::RG: return GL_RG;
            case Texture::Channels::RGB: return GL_RGB;
            case Texture::Channels::RGBA: return GL_RGBA;
            case Texture::Channels::Depth: return GL_DEPTH_COMPONENT;
            case Texture::Channels::None: DE_ASSERT(false, "Texture channel not specified"); break;
            default: DE_ASSERT(false, "Unsupported texture channel");
        }
        return 0;
    }
    GLenum GLDataType(TextureFormat format) {
        switch (format) {
            case TextureFormat::U8: return GL_UNSIGNED_BYTE;
            case TextureFormat::Float: return GL_FLOAT;
            case TextureFormat::None: DE_ASSERT(false, "TextureFormat not specified"); break;
            default: DE_ASSERT(false, "Unsupported TextureFormat");
        }
        return 0;
    }
    GLenum RenderPrimitiveToGL(RenderPrimitive primitive) {
        switch (primitive) {
            case RenderPrimitive::Triangle: return GL_TRIANGLES;
            case RenderPrimitive::Point: return GL_POINTS;
            case RenderPrimitive::None: DE_ASSERT(false, "TexturePrimitive not specified"); break;
            default: DE_ASSERT(false, "Unsupported RenderPrimitive");
        }
        return 0;
    }
}  // namespace DE