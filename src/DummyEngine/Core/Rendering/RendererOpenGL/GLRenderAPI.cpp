#include "Core/Rendering/RendererOpenGL/GLRenderAPI.h"
#include "ToolBox/Dev/Logger.h"

namespace DE
{

    void GLRenderAPI::SetDefaultState()
    {
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_MULTISAMPLE);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        // glFrontFace(GL_CCW);
        // glDepthMask(GL_TRUE);
        // glDepthFunc(GL_LEQUAL);
        // glEnable(GL_BLEND);
        // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    bool GLRenderAPI::Load(const Window* window)
    {
        window->MakeCurrentContext();
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            Logger::Fatal("loading", "GLRenderer", "Failed to load OpenGL for window: " + window->GetState().name);
            DE_ASSERT(false, "Failed to load OpenGL for window:" + window->GetState().name);
            return false;
        }
        Logger::Info("loading", "GLRenderer", "OpenGL loaded for window: " + window->GetState().name);
        return true;
    }
    void GLRenderAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
    {
        glViewport(x, y, width, height);
    }

    void GLRenderAPI::Clear()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    void GLRenderAPI::DrawIndexed(const Ref<VertexArray>& vertexArray)
    {
        vertexArray->Bind();
        glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->IndicesAmount(), GL_UNSIGNED_INT, nullptr);
    }

    void GLRenderAPI::SetClearColor(const Vec4& color)
    {
        glClearColor(color.x, color.y, color.z, color.w);
    }
    void GLRenderAPI::SetLineWidth(float width)
    {
        glLineWidth(width);
    }
    void GLRenderAPI::Enable(RenderSetting setting)
    {
        switch (setting)
        {
            case RenderSetting::DepthTest: glEnable(GL_DEPTH_TEST);
            case RenderSetting::MultiSampling: glEnable(GL_MULTISAMPLE);
        }
    }
    void GLRenderAPI::Disable(RenderSetting setting)
    {
        switch (setting)
        {
            case RenderSetting::DepthTest: glDisable(GL_DEPTH_TEST);
            case RenderSetting::MultiSampling: glDisable(GL_MULTISAMPLE);
        }
    }

    API GLRenderAPI::GetAPI()
    {
        return API::OpenGL;
    }
}  // namespace DE