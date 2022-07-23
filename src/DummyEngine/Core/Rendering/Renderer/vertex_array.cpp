#include "DummyEngine/Core/Rendering/Renderer/vertex_array.h"
#include "DummyEngine/Core/Rendering/Renderer/renderer.h"
#include "DummyEngine/Core/Rendering/RendererOpenGL/gl_vao.h"

namespace DE
{
    Ref<VertexArray> VertexArray::Create()
    {
        switch (Renderer::CurrentAPI())
        {
            case API::OpenGL: return CreateRef<GLVertexArray>();
            case API::Vulkan: return nullptr;
            case API::None: return nullptr;
        }
        return nullptr;
    }
}  // namespace DE