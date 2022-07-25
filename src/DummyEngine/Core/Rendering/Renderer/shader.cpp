#include "Core/Rendering/Renderer/Shader.h"
#include "Core/Rendering/Renderer/Renderer.h"
#include "Core/Rendering/RendererOpenGL/GLShader.h"

namespace DE
{

    Ref<Shader> Shader::Create(std::string name, std::initializer_list<ShaderPart> initializers)
    {
        switch (Renderer::CurrentAPI())
        {
            case API::OpenGL: return CreateRef<GLShader>(name, initializers);
            case API::Vulkan: {
                DE_ASSERT(false, "Attempt to create Shader on VulkanAPI which is currently unsupported.");
                return nullptr;
                break;
            }
            case API::None: {
                DE_ASSERT(false, "Attempt to create Shader without RenderingAPI. Current API: None.");
                return nullptr;
                break;
            }
        }
        return nullptr;
    }

}  // namespace DE