#include "DummyEngine/Core/Rendering/RendererOpenGL/GLRenderAPI.h"

#include <glad/glad.h>

#include "DummyEngine/Core/Rendering/RendererOpenGL/GLUtils.h"

namespace DE {

    void GLRenderAPI::SetDefaultState() {
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_MULTISAMPLE);
//        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);
//        glFrontFace(GL_CW);
        // glDepthMask(GL_TRUE);
        // glDepthFunc(GL_LEQUAL);

        glEnable(GL_BLEND);
        glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBlendEquation(GL_FUNC_ADD);
        glClearColor(0, 0, 0, 1);
    }

    void GLRenderAPI::SetViewport(U32 x, U32 y, U32 width, U32 height) {
        glViewport(x, y, width, height);
    }

    void GLRenderAPI::Clear() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    }

    void GLRenderAPI::DrawArrays(const Ref<VertexArray>& vertex_array, RenderPrimitive primitive) {
        vertex_array->Bind();
        glDrawArrays(RenderPrimitiveToGL(primitive), 0, vertex_array->GetVertexBuffers()[0]->Size());
    }
    void GLRenderAPI::DrawIndexed(const Ref<VertexArray>& vertex_array, RenderPrimitive primitive) {
        vertex_array->Bind();
        glDrawElements(RenderPrimitiveToGL(primitive), vertex_array->GetIndexBuffer()->IndicesAmount(), GL_UNSIGNED_INT, nullptr);
    }
    void GLRenderAPI::DrawInstanced(const Ref<VertexArray>& vertex_array, U32 instance_count, RenderPrimitive primitive) {
        vertex_array->Bind();
        glDrawElementsInstanced(
            RenderPrimitiveToGL(primitive), vertex_array->GetIndexBuffer()->IndicesAmount(), GL_UNSIGNED_INT, nullptr, instance_count);
    }

    void GLRenderAPI::SetDefaultFrameBuffer() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void GLRenderAPI::SetClearColor(const Vec4& color) {
        glClearColor(color.x, color.y, color.z, color.w);
    }
    void GLRenderAPI::SetLineWidth(float width) {
        glLineWidth(width);
    }
    void GLRenderAPI::Enable(RenderSetting setting) {
        switch (setting) {
            case RenderSetting::DepthTest: glEnable(GL_DEPTH_TEST); break;
            case RenderSetting::MultiSampling: glEnable(GL_MULTISAMPLE);  break;
            case RenderSetting::FaceCulling: glEnable(GL_CULL_FACE);  break;
            case RenderSetting::DepthMask: glDepthMask(GL_TRUE);  break;
            default: break;
        }
    }
    void GLRenderAPI::Disable(RenderSetting setting) {
        switch (setting) {
            case RenderSetting::DepthTest: glDisable(GL_DEPTH_TEST);  break;
            case RenderSetting::MultiSampling: glDisable(GL_MULTISAMPLE);  break;
            case RenderSetting::FaceCulling: glDisable(GL_CULL_FACE);  break;
            case RenderSetting::DepthMask: glDepthMask(GL_FALSE);  break;
            default: break;
        }
    }

    API GLRenderAPI::GetAPI() {
        return API::OpenGL;
    }
}  // namespace DE