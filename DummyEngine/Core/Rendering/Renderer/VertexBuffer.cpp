#include "VertexBuffer.h"

#include "DummyEngine/Core/Rendering/Renderer/Renderer.h"
#include "DummyEngine/Core/Rendering/RendererOpenGL/GLVertexBuffer.h"
#include "DummyEngine/Utils/Debug/Assert.h"

namespace DummyEngine {

    Ref<VertexBuffer> VertexBuffer::Create(const BufferLayout& layout, U32 size, BufferUsage usage) {
        switch (Renderer::CurrentAPI()) {
            case API::OpenGL: return CreateRef<GLVertexBuffer>(layout, size, usage);
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
    Ref<VertexBuffer> VertexBuffer::Create(const BufferLayout& layout, U32 size, const void* data, BufferUsage usage) {
        switch (Renderer::CurrentAPI()) {
            case API::OpenGL: return CreateRef<GLVertexBuffer>(layout, size, data, usage);
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

    Ref<IndexBuffer> IndexBuffer::Create(const U32* indices, U32 count) {
        switch (Renderer::CurrentAPI()) {
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

}  // namespace DummyEngine