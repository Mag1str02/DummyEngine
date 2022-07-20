#include "DummyEngine/Core/Rendering/RendererOpenGL/gl_renderer.h"
#include "DummyEngine/ToolBox/Dev/logger.h"

namespace DE {

bool GLRenderer::Load(const Window* window) {
    window->MakeCurrentContext();
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        Logger::Fatal("loading", "GLRenderer", "Failed to load OpenGL for window: " + window->GetState().name);
        return false;
    }
    Logger::Info("loading", "GLRenderer", "OpenGL loaded for window: " + window->GetState().name);
    return true;
}
void GLRenderer::SetViewport(uint32_t width, uint32_t height, uint32_t x_offset, uint32_t y_offset) {
    glViewport(x_offset, y_offset, width, height);
}

void GLRenderer::SetDefaultState() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    // glFrontFace(GL_CCW);
    // glCullFace(GL_BACK);
    // glEnable(GL_CULL_FACE);
    // glDepthMask(GL_TRUE);
    // glDepthFunc(GL_LEQUAL);
    // glEnable(GL_BLEND);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void GLRenderer::Clear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}
void GLRenderer::SetClearColor(glm::vec4 color) {
    glClearColor(color.r, color.g, color.b, color.a);
}
void GLRenderer::SetClearColor(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
}

}  // namespace DE