#include "VertexArray.h"

#include "DummyEngine/Core/Rendering/Renderer/Renderer.h"
#include "DummyEngine/Core/Rendering/RendererOpenGL/GLVertexArray.h"

namespace DummyEngine {

    Ref<VertexArray> VertexArray::Copy() const {
        Ref<VertexArray> result = VertexArray::Create();
        for (const auto& v : GetVertexBuffers()) {
            result->AddVertexBuffer(v);
        }
        result->SetIndexBuffer(GetIndexBuffer());
        return result;
    }
    Ref<VertexArray> VertexArray::Create() {
        switch (Renderer::CurrentAPI()) {
            case API::OpenGL: return CreateRef<GLVertexArray>();
            case API::Vulkan: {
                DE_ASSERT(false, "Attempt to create VertexArray on VulkanAPI which is currently unsupported.");
                return nullptr;
                break;
            }
            case API::None: {
                DE_ASSERT(false, "Attempt to create VertexArray without RenderingAPI. Current API: None.");
                return nullptr;
                break;
            }
        }
        return nullptr;
    }
}  // namespace DummyEngine