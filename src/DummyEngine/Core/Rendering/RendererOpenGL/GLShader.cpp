#include "DummyEngine/Core/Rendering/RendererOpenGL/GLShader.h"
#include "DummyEngine/ToolBox/Dev/Logger.h"
#include "DummyEngine/Core/Rendering/Renderer/Renderer.h"

namespace DE
{
    GLenum ShaderPartTypeToGLShaderPartType(ShaderPartType type)
    {
        switch (type)
        {
            case ShaderPartType::Vertex: return GL_VERTEX_SHADER;
            case ShaderPartType::Fragment: return GL_FRAGMENT_SHADER;
            case ShaderPartType::Geometry: return GL_GEOMETRY_SHADER;
            default: return GL_VERTEX_SHADER;
        }
    }

    GLShader::GLShader(const std::vector<ShaderPart>& shader_parts)
    {
        LOG_INFO("Loading shader.", "GLShader");
        m_ShaderId = glCreateProgram();
        for (const auto& part : shader_parts)
        {
            AddPart(part);
        }
        glLinkProgram(m_ShaderId);

        int  success;
        char info_log[Config::GetI(DE_CFG_MAX_COMPILE_ERROR_LEN)];
        glGetProgramiv(m_ShaderId, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(m_ShaderId, 512, NULL, info_log);
            LOG_FATAL(StrCat("Failed to link shader program (", std::to_string(m_ShaderId), ")\n", reinterpret_cast<const char*>(&info_log)),
                      "GLShader");
            throw std::runtime_error("Failed to compile shader.");
        }
        LOG_INFO(StrCat("GLShader program (", std::to_string(m_ShaderId), ") linked successfully"), "GLShader");
    }
    GLShader::~GLShader()
    {
        glDeleteProgram(m_ShaderId);
        for (const auto& part : m_Parts)
        {
            glDeleteShader(part);
        }
    }

    void GLShader::Bind() const { glUseProgram(m_ShaderId); }
    void GLShader::UnBind() const { glUseProgram(0); }

    void GLShader::SetFloat(const std::string& uniform_name, float x) const
    {
        GLint pos = glGetUniformLocation(m_ShaderId, uniform_name.c_str());
        glUniform1f(pos, x);
    }
    void GLShader::SetFloat2(const std::string& uniform_name, float x, float y) const
    {
        GLint pos = glGetUniformLocation(m_ShaderId, uniform_name.c_str());
        glUniform2f(pos, x, y);
    }
    void GLShader::SetFloat3(const std::string& uniform_name, float x, float y, float z) const
    {
        GLint pos = glGetUniformLocation(m_ShaderId, uniform_name.c_str());
        glUniform3f(pos, x, y, z);
    }
    void GLShader::SetFloat4(const std::string& uniform_name, float x, float y, float z, float w) const
    {
        GLint pos = glGetUniformLocation(m_ShaderId, uniform_name.c_str());
        glUniform4f(pos, x, y, z, w);
    }
    void GLShader::SetFloat2(const std::string& uniform_name, Vec2 value) const
    {
        GLint pos = glGetUniformLocation(m_ShaderId, uniform_name.c_str());
        glUniform2f(pos, value.x, value.y);
    }
    void GLShader::SetFloat3(const std::string& uniform_name, Vec3 value) const
    {
        GLint pos = glGetUniformLocation(m_ShaderId, uniform_name.c_str());
        glUniform3f(pos, value.x, value.y, value.z);
    }
    void GLShader::SetFloat4(const std::string& uniform_name, Vec4 value) const
    {
        GLint pos = glGetUniformLocation(m_ShaderId, uniform_name.c_str());
        glUniform4f(pos, value.x, value.y, value.z, value.w);
    }
    void GLShader::SetInt(const std::string& uniform_name, int x) const
    {
        GLint pos = glGetUniformLocation(m_ShaderId, uniform_name.c_str());
        glUniform1i(pos, x);
    }
    void GLShader::SetInt2(const std::string& uniform_name, int x, int y) const
    {
        GLint pos = glGetUniformLocation(m_ShaderId, uniform_name.c_str());
        glUniform2i(pos, x, y);
    }
    void GLShader::SetInt3(const std::string& uniform_name, int x, int y, int z) const
    {
        GLint pos = glGetUniformLocation(m_ShaderId, uniform_name.c_str());
        glUniform3i(pos, x, y, z);
    }
    void GLShader::SetInt4(const std::string& uniform_name, int x, int y, int z, int w) const
    {
        GLint pos = glGetUniformLocation(m_ShaderId, uniform_name.c_str());
        glUniform4i(pos, x, y, z, w);
    }
    void GLShader::SetMat4(const std::string& uniform_name, Mat4 value) const
    {
        GLint pos = glGetUniformLocation(m_ShaderId, uniform_name.c_str());
        glUniformMatrix4fv(pos, 1, GL_FALSE, glm::value_ptr(value));
    }
    void GLShader::SetUnifromBlock(const std::string& uniform_name, uint32_t id) const
    {
        GLint pos = glGetUniformBlockIndex(m_ShaderId, uniform_name.c_str());
        glUniformBlockBinding(m_ShaderId, pos, id);
        glCheckError();
    }
    void GLShader::SetMaterial(const std::string& uniform_name, const Material& mat) const
    {
        SetFloat3(uniform_name + ".m_Ambient", mat.ambient_color);
        SetFloat3(uniform_name + ".m_Diffuse", mat.diffuse_color);
        SetFloat3(uniform_name + ".m_Specular", mat.specular_color);
        SetFloat(uniform_name + ".m_Shininess", mat.shininess);

        Renderer::GetDefaultTexture()->Bind(0);
        if (mat.specular_map)
        {
            SetInt(uniform_name + ".m_SpecularMap", 1);
            mat.specular_map->Bind(1);
        }
        else
        {
            SetInt(uniform_name + ".m_SpecularMap", 0);
        }
        if (mat.diffuse_map)
        {
            SetInt(uniform_name + ".m_DiffuseMap", 2);
            mat.diffuse_map->Bind(2);
        }
        else
        {
            SetInt(uniform_name + ".m_DiffuseMap", 0);
        }
    }

    std::string GLShader::ReadPartFromFile(const Path& path_to_file)
    {
        std::string line;
        std::string source_string;

        std::ifstream fin(path_to_file);
        if (!fin.is_open())
        {
            LOG_ERROR(StrCat("Can't open shader source file: (", path_to_file.string(), ")"), "GLShader");
            return source_string;
        }
        try
        {
            while (getline(fin, line))
            {
                source_string.append(line + "\n");
            }
        } catch (...)
        {
            LOG_ERROR(StrCat("Failed to read shader source file: (", path_to_file.string(), ")"), "GLShader");
            return source_string;
        }
        LOG_INFO(StrCat("GLShader source file readed: (", path_to_file.string(), ")"), "GLShader");
        return source_string;
    }
    void GLShader::AddPart(ShaderPart part)
    {
        std::string source       = ReadPartFromFile(part.path);
        const char* source_c_str = source.c_str();

        GLuint shader_part = glCreateShader(ShaderPartTypeToGLShaderPartType(part.type));
        glShaderSource(shader_part, 1, &source_c_str, NULL);
        glCompileShader(shader_part);
        m_Parts.push_back(shader_part);

        int  success = 1;
        char infoLog[Config::GetI(DE_CFG_MAX_COMPILE_ERROR_LEN)];
        glGetShaderiv(shader_part, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader_part, Config::GetI(DE_CFG_MAX_COMPILE_ERROR_LEN), NULL, infoLog);
            LOG_ERROR(StrCat("Failed to compile shader: (", part.path.string(), ")\n", reinterpret_cast<const char*>(&infoLog)), "GLShader");
            return;
        }
        LOG_INFO(StrCat("GLShader source file successfully compiled: (", part.path.string(), ")"), "GLShader");

        glAttachShader(m_ShaderId, shader_part);
    }
}  // namespace DE