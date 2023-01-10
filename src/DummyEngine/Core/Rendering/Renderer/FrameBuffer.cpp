#include "DummyEngine/Core/Rendering/Renderer/FrameBuffer.h"
#include "DummyEngine/Core/Rendering/Renderer/Renderer.h"
#include "DummyEngine/Core/Rendering/RendererOpenGL/GLFrameBuffer.h"

namespace DE
{
    Ref<FrameBuffer> FrameBuffer::Create(const FrameBufferProperties& properties)
    {
        switch (Renderer::CurrentAPI())
        {
            case API::OpenGL: return CreateRef<GLFrameBuffer>(properties);
            case API::Vulkan: {
                DE_ASSERT(false, "Attempt to create FrameBuffer on VulkanAPI which is currently unsupported.");
                return nullptr;
                break;
            }
            case API::None: {
                DE_ASSERT(false, "Attempt to create FrameBuffer without RenderingAPI. Current API: None.");
                return nullptr;
                break;
            }
        }
        return nullptr;
    }
}  // namespace DE