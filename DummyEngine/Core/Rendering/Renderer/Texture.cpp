#include "DummyEngine/Core/Rendering/Renderer/Texture.h"
#include "DummyEngine/Core/Rendering/RendererOpenGL/GLTexture.h"
#include "DummyEngine/Core/Rendering/Renderer/Renderer.h"

namespace DE
{
    Ref<Texture> Texture::Create(U32 widht, U32 height, TextureFormat format)
    {
        switch (Renderer::CurrentAPI())
        {
            case API::OpenGL: return CreateRef<GLTexture>(widht, height, format);
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
    Ref<Texture> Texture::Create(const TextureData& texture_data)
    {
        switch (Renderer::CurrentAPI())
        {
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

}  // namespace DE