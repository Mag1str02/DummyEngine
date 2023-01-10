#include "DummyEngine/Core/Rendering/Renderer/Context.h"
#include "DummyEngine/Core/Rendering/Renderer/Renderer.h"
#include "DummyEngine/Core/Rendering/RendererOpenGL/GLContext.h"

namespace DE
{

    Scope<Context> Context::Create(GLFWwindow* window)
    {
        switch (Config::GetRenderAPI())
        {
            case API::OpenGL: return CreateScope<GLContext>(window);
            case API::Vulkan: {
                DE_ASSERT(false, "Attempt to create Context on VulkanAPI which is currently unsupported.");
                return nullptr;
                break;
            }
            case API::None: {
                DE_ASSERT(false, "Attempt to create Context without RenderingAPI. Current API: None.");
                return nullptr;
                break;
            }
        }
        return nullptr;
    }
}  // namespace DE