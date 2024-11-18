#include "DummyEngine/Core/Rendering/RendererOpenGL/GLContext.h"

#include "DummyEngine/Core/Rendering/RendererOpenGL/GLUtils.h"

// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

namespace DE {

    GLContext::GLContext(GLFWwindow* window) : m_Window(window) {
        DE_ASSERT(window, "Wrong window handle");
    }

    void GLContext::Load() {
        glfwMakeContextCurrent(m_Window);
        int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        DE_ASSERT(status, "Failed to initialize GLAD");

        LOG_INFO("OpenGL loaded for context");
        LOG_INFO("Vendor {}, Renderer {}, Version {}", GL_STRING(GL_VENDOR), GL_STRING(GL_RENDERER), GL_STRING(GL_VERSION));
    }

    void GLContext::SwapBuffers() {
        DE_PROFILE_SCOPE("Context SwapBuffers");

        glfwSwapBuffers(m_Window);
    }

}  // namespace DE
