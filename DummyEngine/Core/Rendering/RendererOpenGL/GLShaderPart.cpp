#include "GLShaderPart.h"

#include "DummyEngine/Core/Application/Config.h"

namespace DE {

    GLenum ShaderPartTypeToGLShaderPartType(ShaderPartType type) {
        switch (type) {
            case ShaderPartType::Vertex: return GL_VERTEX_SHADER;
            case ShaderPartType::Fragment: return GL_FRAGMENT_SHADER;
            case ShaderPartType::Geometry: return GL_GEOMETRY_SHADER;
            default: return GL_VERTEX_SHADER;
        }
    }

    GLShaderPart::GLShaderPart(const ShaderPartResource& data) {
        const char* source_c_str = data.Source().c_str();

        m_ShaderId = glCreateShader(ShaderPartTypeToGLShaderPartType(data.Type()));
        glShaderSource(m_ShaderId, 1, &source_c_str, nullptr);
        glCompileShader(m_ShaderId);

        int  success = 1;
        char infoLog[Config::GetI(DE_CFG_MAX_COMPILE_ERROR_LEN)];
        glGetShaderiv(m_ShaderId, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(m_ShaderId, Config::GetI(DE_CFG_MAX_COMPILE_ERROR_LEN), nullptr, infoLog);
            LOG_ERROR("GLShaderPart", "Failed to compile shader (", data.GetID(), ")\n", reinterpret_cast<const char*>(&infoLog));
            return;
        }
        LOG_INFO("GLShaderPart", "Shader (", data.GetID(), ") compiled");
    }
    GLShaderPart::~GLShaderPart() {
        glDeleteShader(m_ShaderId);
    }
}  // namespace DE