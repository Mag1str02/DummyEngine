#include "Context.h"

#include "DummyEngine/Core/Application/Config.h"
#include "DummyEngine/Core/Rendering/RendererOpenGL/GLContext.h"
#include "DummyEngine/Utils/Debug/Assert.h"

namespace DummyEngine {

    Scope<Context> Context::Create(GLFWwindow* window) {
        switch (Config::Get().RendererAPI) {  // Use config because renderer is not yet initialized
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
}  // namespace DummyEngine