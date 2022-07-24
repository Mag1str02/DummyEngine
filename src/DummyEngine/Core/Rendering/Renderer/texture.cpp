#include "DummyEngine/Core/Rendering/Renderer/Texture.h"
#include "DummyEngine/Core/Rendering/RendererOpenGL/GLTexture.h"
#include "DummyEngine/Core/Rendering/Renderer/Renderer.h"

namespace DE
{
    Ref<Texture> Texture::Create(uint32_t widht, uint32_t height, TextureFormat format)
    {
        switch (Renderer::CurrentAPI())
        {
            case API::OpenGL: return CreateRef<GLTexture>(widht, height, format);
            case API::Vulkan: return nullptr;
            case API::None: return nullptr;
        }
        return nullptr;
    }
    Ref<Texture> Texture::Create(const TextureData& texture_data)
    {
        switch (Renderer::CurrentAPI())
        {
            case API::OpenGL: return CreateRef<GLTexture>(texture_data);
            case API::Vulkan: return nullptr;
            case API::None: return nullptr;
        }
        return nullptr;
    }

}  // namespace DE