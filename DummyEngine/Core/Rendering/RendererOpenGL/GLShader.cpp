#include "GLShader.h"

#include "DummyEngine/Core/Application/Config.h"
#include "DummyEngine/Core/Rendering/RendererOpenGL/GLDebug.h"

#include <glm/gtc/type_ptr.hpp>

namespace DummyEngine {
    GLenum ShaderPartTypeToGLShaderPartType(ShaderPartType type) {
        switch (type) {
            case ShaderPartType::Vertex: return GL_VERTEX_SHADER;
            case ShaderPartType::Fragment: return GL_FRAGMENT_SHADER;
            case ShaderPartType::Geometry: return GL_GEOMETRY_SHADER;
            default: return GL_VERTEX_SHADER;
        }
    }

    GLShader::GLShader(const std::vector<ShaderPart>& shader_parts) {
        shader_id_ = glCreateProgram();
        for (const auto& part : shader_parts) {
            AddPart(part);
        }
        glLinkProgram(shader_id_);

        int         success;
        std::string info_log;
        info_log.resize(Config::Get().MaxShaderCompileErrorLen);
        glGetProgramiv(shader_id_, GL_LINK_STATUS, &success);
        int len = 0;
        if (success == GL_FALSE) {
            glGetProgramInfoLog(shader_id_, info_log.size(), &len, info_log.data());
            info_log.resize(len);
            LOG_ERROR("Failed to link shader program {} due:\n{}", std::to_string(shader_id_), info_log);
            throw std::runtime_error("Failed to compile shader.");
        }
        LOG_INFO("GLShader program {} linked successfully", std::to_string(shader_id_));
    }
    GLShader::~GLShader() {
        glDeleteProgram(shader_id_);
        for (const auto& part : parts_) {
            glDeleteShader(part);
        }
    }

    void GLShader::Bind() const {
        glUseProgram(shader_id_);
    }
    void GLShader::UnBind() const {
        glUseProgram(0);
    }

    void GLShader::SetFloat(const std::string& uniform_name, float x) const {
        GLint pos = glGetUniformLocation(shader_id_, uniform_name.c_str());
        glUniform1f(pos, x);
    }
    void GLShader::SetFloat2(const std::string& uniform_name, float x, float y) const {
        GLint pos = glGetUniformLocation(shader_id_, uniform_name.c_str());
        glUniform2f(pos, x, y);
    }
    void GLShader::SetFloat3(const std::string& uniform_name, float x, float y, float z) const {
        GLint pos = glGetUniformLocation(shader_id_, uniform_name.c_str());
        glUniform3f(pos, x, y, z);
    }
    void GLShader::SetFloat4(const std::string& uniform_name, float x, float y, float z, float w) const {
        GLint pos = glGetUniformLocation(shader_id_, uniform_name.c_str());
        glUniform4f(pos, x, y, z, w);
    }
    void GLShader::SetFloat2(const std::string& uniform_name, Vec2 value) const {
        GLint pos = glGetUniformLocation(shader_id_, uniform_name.c_str());
        glUniform2f(pos, value.x, value.y);
    }
    void GLShader::SetFloat3(const std::string& uniform_name, Vec3 value) const {
        GLint pos = glGetUniformLocation(shader_id_, uniform_name.c_str());
        glUniform3f(pos, value.x, value.y, value.z);
    }
    void GLShader::SetFloat4(const std::string& uniform_name, Vec4 value) const {
        GLint pos = glGetUniformLocation(shader_id_, uniform_name.c_str());
        glUniform4f(pos, value.x, value.y, value.z, value.w);
    }
    void GLShader::SetInt(const std::string& uniform_name, int x) const {
        GLint pos = glGetUniformLocation(shader_id_, uniform_name.c_str());
        glUniform1i(pos, x);
    }
    void GLShader::SetInt2(const std::string& uniform_name, int x, int y) const {
        GLint pos = glGetUniformLocation(shader_id_, uniform_name.c_str());
        glUniform2i(pos, x, y);
    }
    void GLShader::SetInt3(const std::string& uniform_name, int x, int y, int z) const {
        GLint pos = glGetUniformLocation(shader_id_, uniform_name.c_str());
        glUniform3i(pos, x, y, z);
    }
    void GLShader::SetInt4(const std::string& uniform_name, int x, int y, int z, int w) const {
        GLint pos = glGetUniformLocation(shader_id_, uniform_name.c_str());
        glUniform4i(pos, x, y, z, w);
    }
    void GLShader::SetMat4(const std::string& uniform_name, Mat4 value) const {
        GLint pos = glGetUniformLocation(shader_id_, uniform_name.c_str());
        glUniformMatrix4fv(pos, 1, GL_FALSE, glm::value_ptr(value));
    }
    void GLShader::SetUnifromBlock(const std::string& uniform_name, U32 id) const {
        GLint pos = glGetUniformBlockIndex(shader_id_, uniform_name.c_str());
        glUniformBlockBinding(shader_id_, pos, id);
        glCheckError();
    }

    std::string GLShader::ReadPartFromFile(const Path& path_to_file) {
        std::string line;
        std::string source_string;

        std::ifstream fin(path_to_file);
        if (!fin.is_open()) {
            LOG_ERROR("Can't open shader source file {}", Config::RelativeToExecutable(path_to_file));
            return source_string;
        }
        try {
            while (getline(fin, line)) {
                source_string.append(line + "\n");
            }
        } catch (...) {
            LOG_ERROR("Failed to read shader source file {}", Config::RelativeToExecutable(path_to_file));
            return source_string;
        }
        return source_string;
    }
    void GLShader::AddPart(ShaderPart part) {
        std::string source       = ReadPartFromFile(part.Path);
        const char* source_c_str = source.c_str();

        GLuint shader_part = glCreateShader(ShaderPartTypeToGLShaderPartType(part.Type));
        glShaderSource(shader_part, 1, &source_c_str, nullptr);
        glCompileShader(shader_part);
        parts_.push_back(shader_part);

        int         success = 1;
        std::string info_log;
        info_log.resize(Config::Get().MaxShaderCompileErrorLen);
        int len = 0;
        glGetShaderiv(shader_part, GL_COMPILE_STATUS, &success);
        if (success == GL_FALSE) {
            glGetShaderInfoLog(shader_part, info_log.size(), &len, info_log.data());
            info_log.resize(len);
            LOG_ERROR("Failed to compile shader {} due:\n{}", part.Path, info_log);
            return;
        }
        LOG_INFO("File {} compiled", Config::RelativeToExecutable(part.Path));

        glAttachShader(shader_id_, shader_part);
    }
}  // namespace DummyEngine