#include "CubeMap.h"

#include "DummyEngine/Core/Rendering/Renderer/Renderer.h"
#include "DummyEngine/Core/Rendering/RendererOpenGL/GLCubeMap.h"

namespace DummyEngine {

    Ref<CubeMap> CubeMap::Create(const Ref<TextureData> texture_data) {
        switch (Renderer::CurrentAPI()) {
            case API::OpenGL: return CreateRef<GLCubeMap>(texture_data);
            case API::Vulkan: {
                DE_ASSERT(false, "Attempt to create CubeMap on VulkanAPI which is currently unsupported.");
                return nullptr;
                break;
            }
            case API::None: {
                DE_ASSERT(false, "Attempt to create CubeMap without RenderingAPI. Current API: None.");
                return nullptr;
                break;
            }
        }
        return nullptr;
    }
    Ref<CubeMap> CubeMap::Create(U32 size, Texture::Format format, Texture::Channels channels, bool gen_mipmap, bool depth_map) {
        switch (Renderer::CurrentAPI()) {
            case API::OpenGL: return CreateRef<GLCubeMap>(size, format, channels, gen_mipmap, depth_map);
            case API::Vulkan: {
                DE_ASSERT(false, "Attempt to create CubeMap on VulkanAPI which is currently unsupported.");
                return nullptr;
                break;
            }
            case API::None: {
                DE_ASSERT(false, "Attempt to create CubeMap without RenderingAPI. Current API: None.");
                return nullptr;
                break;
            }
        }
        return nullptr;
    }

}  // namespace DummyEngine