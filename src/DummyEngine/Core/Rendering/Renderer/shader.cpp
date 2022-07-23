#include "DummyEngine/Core/Rendering/Renderer/shader.h"
#include "DummyEngine/Core/Rendering/Renderer/renderer.h"
#include "DummyEngine/Core/Rendering/RendererOpenGL/gl_shader.h"

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