#include "DummyEngine/Core/Rendering/RendererOpenGL/GLShader.h"

#include "DummyEngine/Core/Application/Config.h"
#include "DummyEngine/Core/Rendering/Renderer/Renderer.h"
#include "DummyEngine/Core/Rendering/RendererOpenGL/GLDebug.h"

namespace DE {

    GLShader::GLShader(const std::vector<Ref<GLShaderPart>>& shader_parts) {
        m_ShaderId = glCreateProgram();
        for (const auto& part : shader_parts) {
            AddPart(part);
        }
        glLinkProgram(m_ShaderId);

        int  success;
        char info_log[Config::GetI(DE_CFG_MAX_COMPILE_ERROR_LEN)];
        glGetProgramiv(m_ShaderId, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(m_ShaderId, 512, NULL, info_log);
            LOG_ERROR("GLShader", "Failed to link shader program (", std::to_string(m_ShaderId), ")\n", reinterpret_cast<const char*>(&info_log));
            throw std::runtime_error("Failed to compile shader.");
        }
        LOG_INFO("GLShader", "GLShader program (", std::to_string(m_ShaderId), ") linked successfully");
    }
    GLShader::~GLShader() {
        glDeleteProgram(m_ShaderId);
    }

    void GLShader::Unload() const {
        glDeleteProgram(m_ShaderId);
    }
    void GLShader::Bind() const {
        glUseProgram(m_ShaderId);
    }
    void GLShader::UnBind() const {
        glUseProgram(0);
    }

    void GLShader::SetFloat(const std::string& uniform_name, float x) const {
        GLint pos = glGetUniformLocation(m_ShaderId, uniform_name.c_str());
        glUniform1f(pos, x);
    }
    void GLShader::SetFloat2(const std::string& uniform_name, float x, float y) const {
        GLint pos = glGetUniformLocation(m_ShaderId, uniform_name.c_str());
        glUniform2f(pos, x, y);
    }
    void GLShader::SetFloat3(const std::string& uniform_name, float x, float y, float z) const {
        GLint pos = glGetUniformLocation(m_ShaderId, uniform_name.c_str());
        glUniform3f(pos, x, y, z);
    }
    void GLShader::SetFloat4(const std::string& uniform_name, float x, float y, float z, float w) const {
        GLint pos = glGetUniformLocation(m_ShaderId, uniform_name.c_str());
        glUniform4f(pos, x, y, z, w);
    }
    void GLShader::SetFloat2(const std::string& uniform_name, Vec2 value) const {
        GLint pos = glGetUniformLocation(m_ShaderId, uniform_name.c_str());
        glUniform2f(pos, value.x, value.y);
    }
    void GLShader::SetFloat3(const std::string& uniform_name, Vec3 value) const {
        GLint pos = glGetUniformLocation(m_ShaderId, uniform_name.c_str());
        glUniform3f(pos, value.x, value.y, value.z);
    }
    void GLShader::SetFloat4(const std::string& uniform_name, Vec4 value) const {
        GLint pos = glGetUniformLocation(m_ShaderId, uniform_name.c_str());
        glUniform4f(pos, value.x, value.y, value.z, value.w);
    }
    void GLShader::SetInt(const std::string& uniform_name, int x) const {
        GLint pos = glGetUniformLocation(m_ShaderId, uniform_name.c_str());
        glUniform1i(pos, x);
    }
    void GLShader::SetInt2(const std::string& uniform_name, int x, int y) const {
        GLint pos = glGetUniformLocation(m_ShaderId, uniform_name.c_str());
        glUniform2i(pos, x, y);
    }
    void GLShader::SetInt3(const std::string& uniform_name, int x, int y, int z) const {
        GLint pos = glGetUniformLocation(m_ShaderId, uniform_name.c_str());
        glUniform3i(pos, x, y, z);
    }
    void GLShader::SetInt4(const std::string& uniform_name, int x, int y, int z, int w) const {
        GLint pos = glGetUniformLocation(m_ShaderId, uniform_name.c_str());
        glUniform4i(pos, x, y, z, w);
    }
    void GLShader::SetMat4(const std::string& uniform_name, Mat4 value) const {
        GLint pos = glGetUniformLocation(m_ShaderId, uniform_name.c_str());
        glUniformMatrix4fv(pos, 1, GL_FALSE, glm::value_ptr(value));
    }
    void GLShader::SetUniformBlock(const std::string& uniform_name, U32 id) const {
        GLint pos = glGetUniformBlockIndex(m_ShaderId, uniform_name.c_str());
        glUniformBlockBinding(m_ShaderId, pos, id);
        glCheckError();
    }

    void GLShader::AddPart(Ref<GLShaderPart> part) const {
        glAttachShader(m_ShaderId, part->m_ShaderId);
    }
}  // namespace DE