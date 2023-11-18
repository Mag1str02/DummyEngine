#pragma once

#include "DummyEngine/Core/ResourceManaging/RawData.h"
#include "DummyEngine/Utils/Base.h"
#include "ShaderPart.h"

namespace DE {

    class Shader {
    public:
        virtual ~Shader() = default;

        virtual void Bind() const   = 0;
        virtual void UnBind() const = 0;
        virtual void Unload() const = 0;

        virtual void SetFloat(const std::string& uniform_name, float value) const                         = 0;
        virtual void SetFloat2(const std::string& uniform_name, float x, float y) const                   = 0;
        virtual void SetFloat3(const std::string& uniform_name, float x, float y, float z) const          = 0;
        virtual void SetFloat4(const std::string& uniform_name, float x, float y, float z, float w) const = 0;
        virtual void SetFloat2(const std::string& uniform_name, Vec2 value) const                         = 0;
        virtual void SetFloat3(const std::string& uniform_name, Vec3 value) const                         = 0;
        virtual void SetFloat4(const std::string& uniform_name, Vec4 value) const                         = 0;
        virtual void SetInt(const std::string& uniform_name, int x) const                                 = 0;
        virtual void SetInt2(const std::string& uniform_name, int x, int y) const                         = 0;
        virtual void SetInt3(const std::string& uniform_name, int x, int y, int z) const                  = 0;
        virtual void SetInt4(const std::string& uniform_name, int x, int y, int z, int w) const           = 0;
        virtual void SetMat4(const std::string& uniform_name, Mat4 value) const                           = 0;
        virtual void SetUniformBlock(const std::string& uniform_name, U32 id) const                       = 0;

        static Ref<Shader> Create(const std::vector<Ref<ShaderPart>>& initializers);
    };
}  // namespace DE