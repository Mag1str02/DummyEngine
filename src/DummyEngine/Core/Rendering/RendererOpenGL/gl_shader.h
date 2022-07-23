#pragma once

#include <GLAD/glad.h>

#include "DummyEngine/Addition/base.h"
#include "DummyEngine/Core/Rendering/Renderer/shader.h"
#include "DummyEngine/Core/Rendering/Renderer/render_data.h"

namespace DE
{
    namespace fs = std::filesystem;

    class GLShader : public Shader
    {
    public:
        GLShader() = delete;
        GLShader(const Shader& other) = delete;
        GLShader(Shader&& other) = delete;
        GLShader& operator=(Shader&& other) = delete;
        GLShader& operator=(const Shader& other) = delete;

        GLShader(const std::string& name, std::initializer_list<ShaderPart>& shader_parts);
        virtual ~GLShader();

        virtual void Bind() const override;
        virtual void UnBind() const override;

        virtual void SetFloat(const std::string& uniform_name, float value) const override;
        virtual void SetFloat2(const std::string& uniform_name, float x, float y) const override;
        virtual void SetFloat3(const std::string& uniform_name, float x, float y, float z) const override;
        virtual void SetFloat4(const std::string& uniform_name, float x, float y, float z, float w) const override;
        virtual void SetFloat2(const std::string& uniform_name, Vec2 value) const override;
        virtual void SetFloat3(const std::string& uniform_name, Vec3 value) const override;
        virtual void SetFloat4(const std::string& uniform_name, Vec4 value) const override;
        virtual void SetInt(const std::string& uniform_name, int x) const override;
        virtual void SetInt2(const std::string& uniform_name, int x, int y) const override;
        virtual void SetInt3(const std::string& uniform_name, int x, int y, int z) const override;
        virtual void SetInt4(const std::string& uniform_name, int x, int y, int z, int w) const override;
        virtual void SetMat4(const std::string& uniform_name, Mat4 value) const override;
        virtual void SetMaterial(const std::string& uniform_name, const Material& mat) const override;

        virtual const std::string& GetName() const;

    private:
        static std::string ReadPartFromFile(const fs::path& path_to_file);

        void AddPart(ShaderPart part);

        std::string _name;
        std::vector<GLuint> _parts;
        GLuint _shader_id;
    };

    // class GLShader
    // {
    // public:
    //     GLShader(const fs::path& path_to_file, GLenum type);
    //     void Init(const fs::path& path_to_file, GLenum type);

    //     GLShader();
    //     GLShader(const GLShader& other);
    //     GLShader(GLShader&& other);
    //     GLShader& operator=(const GLShader& other);
    //     GLShader& operator=(GLShader&& other);
    //     ~GLShader();

    // private:
    //     unsigned int _shader_id;

    //     std::string ReadShaderFile(const fs::path& path_to_file);
    //     void CheckShader(const fs::path& path_to_file);
    // };
}  // namespace DE