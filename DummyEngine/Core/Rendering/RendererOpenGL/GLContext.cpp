#include "GLContext.h"

#include "DummyEngine/Core/Rendering/RendererOpenGL/GLUtils.h"
#include "DummyEngine/Utils/Debug/Profiler.h"

// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

namespace DummyEngine {

    GLContext::GLContext(GLFWwindow* window) : window_(window) {
        DE_ASSERT(window, "Wrong window handle");
    }

    void GLContext::Load() {
        glfwMakeContextCurrent(window_);
        int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        DE_ASSERT(status, "Failed to initialize GLAD");

        LOG_INFO("OpenGL loaded for context");
        LOG_INFO("Vendor {}, Renderer {}, Version {}", GL_STRING(GL_VENDOR), GL_STRING(GL_RENDERER), GL_STRING(GL_VERSION));
    }

    void GLContext::SwapBuffers() {
        DE_PROFILE_SCOPE("Context SwapBuffers");

        glfwSwapBuffers(window_);
    }

}  // namespace DummyEngine
