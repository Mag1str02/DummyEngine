#pragma once

#include "DummyEngine/Addition/Base.h"
#include "DummyEngine/Core/Rendering/Renderer/RenderStructs.h"

namespace DE
{
    namespace fs = std::filesystem;
    enum class ShaderPartType
    {
        None = 0,
        Vertex,
        Geometry,
        Fragment
    };

    struct ShaderPart
    {
        ShaderPartType type;
        fs::path path;
    };

    class Shader
    {
    public:
        virtual ~Shader() = default;

        virtual void Bind() const = 0;
        virtual void UnBind() const = 0;

        virtual void SetFloat(const std::string& uniform_name, float value) const = 0;
        virtual void SetFloat2(const std::string& uniform_name, float x, float y) const = 0;
        virtual void SetFloat3(const std::string& uniform_name, float x, float y, float z) const = 0;
        virtual void SetFloat4(const std::string& uniform_name, float x, float y, float z, float w) const = 0;
        virtual void SetFloat2(const std::string& uniform_name, Vec2 value) const = 0;
        virtual void SetFloat3(const std::string& uniform_name, Vec3 value) const = 0;
        virtual void SetFloat4(const std::string& uniform_name, Vec4 value) const = 0;
        virtual void SetInt(const std::string& uniform_name, int x) const = 0;
        virtual void SetInt2(const std::string& uniform_name, int x, int y) const = 0;
        virtual void SetInt3(const std::string& uniform_name, int x, int y, int z) const = 0;
        virtual void SetInt4(const std::string& uniform_name, int x, int y, int z, int w) const = 0;
        virtual void SetMat4(const std::string& uniform_name, Mat4 value) const = 0;
        virtual void SetMaterial(const std::string& uniform_name, const Material& mat) const = 0;

        virtual const std::string& GetName() const = 0;

        static Ref<Shader> Create(std::string name, std::initializer_list<ShaderPart> initializers);
    };
}  // namespace DE