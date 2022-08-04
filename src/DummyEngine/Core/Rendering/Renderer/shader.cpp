#include "Core/Rendering/Renderer/Shader.h"
#include "Core/Rendering/Renderer/Renderer.h"
#include "Core/Rendering/RendererOpenGL/GLShader.h"

namespace DE
{
    std::string ShaderPartTypeToString(ShaderPartType type)
    {
        switch (type)
        {
            case ShaderPartType::Vertex: return "Vertex";
            case ShaderPartType::Fragment: return "Fragment";
            case ShaderPartType::Geometry: return "Geometry";
            default: return "None";
        }
    }
    ShaderPartType StringToShaderPartType(const std::string& type)
    {
        if (type == "Vertex") return ShaderPartType::Vertex;
        if (type == "Fragment") return ShaderPartType::Fragment;
        if (type == "Geometry") return ShaderPartType::Geometry;
        return ShaderPartType::None;
    }

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