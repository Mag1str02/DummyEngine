//
// Created by balanda on 10/17/2023.
//

#include "ShaderPart.h"

#include "Renderer.h"

namespace DE {
    Ref<ShaderPart> ShaderPart::Create(const ShaderPartResource& data) {
        switch (Renderer::CurrentAPI()) {
            case API::OpenGL: return CreateRef<GLShaderPart>(initializers);
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