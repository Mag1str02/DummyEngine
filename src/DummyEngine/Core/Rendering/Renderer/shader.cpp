#include "DummyEngine/Core/Rendering/Renderer/Shader.h"
#include "DummyEngine/Core/Rendering/Renderer/Renderer.h"
#include "DummyEngine/Core/Rendering/RendererOpenGL/GLShader.h"

namespace DE
{

    Ref<Shader> Shader::Create(std::string name, std::initializer_list<ShaderPart> initializers)
    {
        switch (Renderer::CurrentAPI())
        {
            case API::OpenGL: return CreateRef<GLShader>(name, initializers);
            default: return nullptr;
        }
    }

}  // namespace DE