#include "Core/Rendering/RendererOpenGL/GLContext.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace DE
{

    GLContext::GLContext(GLFWwindow* window) : m_Window(window)
    {
        DE_ASSERT(window != NULL, "Wrong window handle.");
    }

    void GLContext::Load()
    {
        glfwMakeContextCurrent(m_Window);
        int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        DE_ASSERT(status, "Failed to initialize Glad!");

        // Logger::Info("loading",
        //              "GLContext",
        //              "OpenGL loaded for context. Vendor: " + std::string(glGetString(GL_VENDOR)) + " Renderer: " + glGetString(GL_RENDERER) +
        //                  " Version: " + glGetString(GL_VERSION));
    }

    void GLContext::SwapBuffers()
    {
        // DE_PROFILE_SCOPE("Context SwapBuffers");

        glfwSwapBuffers(m_Window);
    }

}  // namespace Hazel
