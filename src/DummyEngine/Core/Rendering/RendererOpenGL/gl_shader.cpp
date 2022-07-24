#include "DummyEngine/Core/Rendering/RendererOpenGL/gl_shader.h"
#include "DummyEngine/ToolBox/Dev/logger.h"

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

    GLShader::GLShader(const std::string& name, std::initializer_list<ShaderPart>& shader_parts)
    {
        _name = name;
        _shader_id = glCreateProgram();
        for (const auto& part : shader_parts)
        {
            AddPart(part);
        }
        glLinkProgram(_shader_id);

        int success;
        char info_log[Config::Get(DE_CFG_MAX_COMPILE_ERROR_LEN)];
        glGetProgramiv(_shader_id, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(_shader_id, 512, NULL, info_log);
            Logger::Fatal("loading",
                          "GLShaderProgram",
                          "Failed to link shader program (" + std::to_string(_shader_id) + ")\n" + info_log);
            throw std::exception();
        }
        Logger::Info(
            "loading", "GLShaderProgram", "GLShader program (" + std::to_string(_shader_id) + ") linked successfully");
    }
    GLShader::~GLShader()
    {
        glDeleteProgram(_shader_id);
        for (const auto& part : _parts)
        {
            glDeleteShader(part);
        }
    }

    void GLShader::Bind() const
    {
        glUseProgram(_shader_id);
    }
    void GLShader::UnBind() const
    {
        glUseProgram(0);
    }

    void GLShader::SetFloat(const std::string& uniform_name, float x) const
    {
        GLint pos = glGetUniformLocation(_shader_id, uniform_name.c_str());
        glUniform1f(pos, x);
    }
    void GLShader::SetFloat2(const std::string& uniform_name, float x, float y) const
    {
        GLint pos = glGetUniformLocation(_shader_id, uniform_name.c_str());
        glUniform2f(pos, x, y);
    }
    void GLShader::SetFloat3(const std::string& uniform_name, float x, float y, float z) const
    {
        GLint pos = glGetUniformLocation(_shader_id, uniform_name.c_str());
        glUniform3f(pos, x, y, z);
    }
    void GLShader::SetFloat4(const std::string& uniform_name, float x, float y, float z, float w) const
    {
        GLint pos = glGetUniformLocation(_shader_id, uniform_name.c_str());
        glUniform4f(pos, x, y, z, w);
    }
    void GLShader::SetFloat2(const std::string& uniform_name, Vec2 value) const
    {
        GLint pos = glGetUniformLocation(_shader_id, uniform_name.c_str());
        glUniform2f(pos, value.x, value.y);
    }
    void GLShader::SetFloat3(const std::string& uniform_name, Vec3 value) const
    {
        GLint pos = glGetUniformLocation(_shader_id, uniform_name.c_str());
        glUniform3f(pos, value.x, value.y, value.z);
    }
    void GLShader::SetFloat4(const std::string& uniform_name, Vec4 value) const
    {
        GLint pos = glGetUniformLocation(_shader_id, uniform_name.c_str());
        glUniform4f(pos, value.x, value.y, value.z, value.w);
    }
    void GLShader::SetInt(const std::string& uniform_name, int x) const
    {
        GLint pos = glGetUniformLocation(_shader_id, uniform_name.c_str());
        glUniform1i(pos, x);
    }
    void GLShader::SetInt2(const std::string& uniform_name, int x, int y) const
    {
        GLint pos = glGetUniformLocation(_shader_id, uniform_name.c_str());
        glUniform2i(pos, x, y);
    }
    void GLShader::SetInt3(const std::string& uniform_name, int x, int y, int z) const
    {
        GLint pos = glGetUniformLocation(_shader_id, uniform_name.c_str());
        glUniform3i(pos, x, y, z);
    }
    void GLShader::SetInt4(const std::string& uniform_name, int x, int y, int z, int w) const
    {
        GLint pos = glGetUniformLocation(_shader_id, uniform_name.c_str());
        glUniform4i(pos, x, y, z, w);
    }
    void GLShader::SetMat4(const std::string& uniform_name, Mat4 value) const
    {
        GLint pos = glGetUniformLocation(_shader_id, uniform_name.c_str());
        glUniformMatrix4fv(pos, 1, GL_FALSE, glm::value_ptr(value));
    }
    void GLShader::SetMaterial(const std::string& uniform_name, const Material& mat) const
    {
        SetFloat3(uniform_name + ".ambient_color", mat.ambient_color.x, mat.ambient_color.y, mat.ambient_color.z);
        SetFloat3(uniform_name + ".diffuse_color", mat.diffuse_color.x, mat.diffuse_color.y, mat.diffuse_color.z);
        SetFloat3(uniform_name + ".specular_color", mat.specular_color.x, mat.specular_color.y, mat.specular_color.z);

        SetInt(uniform_name + ".shininess", mat.shininess);
        SetInt(uniform_name + ".specular_map", 1);
        SetInt(uniform_name + ".diffuse_map", 2);
        SetInt(uniform_name + ".normal_map", 3);

        mat.specular_map->Bind(1);
        mat.diffuse_map->Bind(2);
        mat.normal_map->Bind(3);
    }

    const std::string& GLShader::GetName() const
    {
        return _name;
    }

    std::string GLShader::ReadPartFromFile(const fs::path& path_to_file)
    {
        std::string line;
        std::string source_string;

        std::ifstream fin(path_to_file);
        if (!fin.is_open())
        {
            Logger::Error("loading", "GLShader", "Can't open shader source file: (" + path_to_file.string() + ")");
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
            Logger::Error("loading", "GLShader", "Failed to read shader source file: (" + path_to_file.string() + ")");
            return source_string;
        }
        Logger::Info("loading", "GLShader", "GLShader source file readed: (" + path_to_file.string() + ")");
        return source_string;
    }
    void GLShader::AddPart(ShaderPart part)
    {
        std::string source = ReadPartFromFile(part.path);
        const char* source_c_str = source.c_str();

        GLuint shader_part = glCreateShader(ShaderPartTypeToGLShaderPartType(part.type));
        glShaderSource(shader_part, 1, &source_c_str, NULL);
        glCompileShader(_shader_id);
        _parts.push_back(shader_part);

        int success = 1;
        char infoLog[Config::Get(DE_CFG_MAX_COMPILE_ERROR_LEN)];
        glGetShaderiv(_shader_id, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(_shader_id, Config::Get(DE_CFG_MAX_COMPILE_ERROR_LEN), NULL, infoLog);
            Logger::Error("loading",
                          "GLShader",
                          "Failed to compile shader: (" + part.path.string() + ")\n" + std::string(infoLog));
            return;
        }
        Logger::Info("loading", "GLShader", "GLShader source file successfully compiled: (" + part.path.string() + ")");

        glAttachShader(_shader_id, shader_part);
    }
}  // namespace DE