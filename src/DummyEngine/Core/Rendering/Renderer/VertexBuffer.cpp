#include "Core/Rendering/Renderer/VertexBuffer.h"
#include "Core/Rendering/Renderer/Renderer.h"
#include "Core/Rendering/RendererOpenGL/GLVertexBuffer.h"

namespace DE
{
    Ref<VertexBuffer> VertexBuffer::Create(uint32_t size, BufferUsage usage)
    {
        switch (Renderer::CurrentAPI())
        {
            case API::OpenGL: return CreateRef<GLVertexBuffer>(size, usage);
            case API::Vulkan: {
                DE_ASSERT(false, "Attempt to create VertexBufffer on VulkanAPI which is currently unsupported.");
                return nullptr;
                break;
            }
            case API::None: {
                DE_ASSERT(false, "Attempt to create VertexBufffer without RenderingAPI. Current API: None.");
                return nullptr;
                break;
            }
        }

        return nullptr;
    }
    Ref<VertexBuffer> VertexBuffer::Create(const void* data, uint32_t size, BufferUsage usage)
    {
        switch (Renderer::CurrentAPI())
        {
            case API::OpenGL: return CreateRef<GLVertexBuffer>(data, size, usage);
            case API::Vulkan: {
                DE_ASSERT(false, "Attempt to create VertexBufffer on VulkanAPI which is currently unsupported.");
                return nullptr;
                break;
            }
            case API::None: {
                DE_ASSERT(false, "Attempt to create VertexBufffer without RenderingAPI. Current API: None.");
                return nullptr;
                break;
            }
        }
        return nullptr;
    }

    Ref<IndexBuffer> IndexBuffer::Create(const uint32_t* indices, uint32_t count)
    {
        switch (Renderer::CurrentAPI())
        {
            case API::OpenGL: return CreateRef<GLIndexBuffer>(indices, count);
            case API::Vulkan: {
                DE_ASSERT(false, "Attempt to create IndexBufffer on VulkanAPI which is currently unsupported.");
                return nullptr;
                break;
            }
            case API::None: {
                DE_ASSERT(false, "Attempt to create IndexBufffer without RenderingAPI. Current API: None.");
                return nullptr;
                break;
            }
        }
        return nullptr;
    }

}  // namespace DE