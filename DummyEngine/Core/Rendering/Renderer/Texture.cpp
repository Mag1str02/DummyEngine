#include "DummyEngine/Core/Rendering/Renderer/Texture.h"

#include "DummyEngine/Core/Rendering/Renderer/Renderer.h"
#include "DummyEngine/Core/Rendering/RendererOpenGL/GLTexture.h"

namespace DE {
    std::string Texture::FormatToStr(Format format) {
        switch (format) {
            case Format::F16: return "F16";
            case Format::F32: return "F32";
            case Format::U8: return "U8";
            case Format::U16: return "U16";
            case Format::None: return "None";
            default: return "Unknown";
        }
    }
    std::string Texture::ChannelsToStr(Channels channel) {
        switch (channel) {
            case Channels::R: return "R";
            case Channels::RG: return "RG";
            case Channels::RGB: return "RGB";
            case Channels::RGBA: return "RGBA";
            case Channels::Depth: return "Depth";
            case Channels::None: return "None";
            default: return "Unknown";
        }
    }

    Ref<Texture> Texture::Create(Texture::Channels channels, Texture::Format format) {
        switch (Renderer::CurrentAPI()) {
            case API::OpenGL: return CreateRef<GLTexture>(channels, format);
            case API::Vulkan: {
                DE_ASSERT(false, "Attempt to create Texture on VulkanAPI which is currently unsupported.");
                return nullptr;
                break;
            }
            case API::None: {
                DE_ASSERT(false, "Attempt to create Texture without RenderingAPI. Current API: None.");
                return nullptr;
                break;
            }
        }
        return nullptr;
    }
    Ref<Texture> Texture::Create(U32 width, U32 height, Texture::Channels channels, Texture::Format format) {
        switch (Renderer::CurrentAPI()) {
            case API::OpenGL: return CreateRef<GLTexture>(width, height, channels, format);
            case API::Vulkan: {
                DE_ASSERT(false, "Attempt to create Texture on VulkanAPI which is currently unsupported.");
                return nullptr;
                break;
            }
            case API::None: {
                DE_ASSERT(false, "Attempt to create Texture without RenderingAPI. Current API: None.");
                return nullptr;
                break;
            }
        }
        return nullptr;
    }
    Ref<Texture> Texture::Create(const TextureResource& texture_data) {
        switch (Renderer::CurrentAPI()) {
            case API::OpenGL: return CreateRef<GLTexture>(texture_data);
            case API::Vulkan: {
                DE_ASSERT(false, "Attempt to create Texture on VulkanAPI which is currently unsupported.");
                return nullptr;
                break;
            }
            case API::None: {
                DE_ASSERT(false, "Attempt to create Texture without RenderingAPI. Current API: None.");
                return nullptr;
                break;
            }
        }
        return nullptr;
    }

    Texture::Format Texture::DataFormat(TextureFormat format) {
        switch (format) {
            case TextureFormat::U8: return Texture::Format::U8;
            case TextureFormat::Float: return Texture::Format::F32;
            case TextureFormat::None: return Texture::Format::None;
            default: DE_ASSERT(false, "Wrong TextureFormat"); break;
        }
        return Texture::Format::None;
    }
    Texture::Channels Texture::DataChannels(TextureChannels channels) {
        switch (channels) {
            case TextureChannels::RED: return Texture::Channels::R;
            case TextureChannels::RG: return Texture::Channels::RG;
            case TextureChannels::RGB: return Texture::Channels::RGB;
            case TextureChannels::RGBA: return Texture::Channels::RGBA;
            case TextureChannels::None: return Texture::Channels::None;
            default: DE_ASSERT(false, "Wrong TextureChannels"); break;
        }
        return Texture::Channels::None;
    }
}  // namespace DE