#pragma once

#include "Addition/Base.h"
#include "Core/Rendering/Renderer/Shader.h"

namespace DE
{
    struct Id
    {
        UUID id;

        Id() = default;
        Id(UUID uuid);
        Id(const Id&) = default;

        operator uint64_t() const
        {
            return id;
        }
    };

    struct Tag
    {
        std::string tag;

        Tag() = default;
        Tag(const std::string& tag);

        operator std::string() const
        {
            return tag;
        }
    };
    struct Transformation
    {
        Vec3 scale = Vec3(1.0), scale_offset = Vec3(0.0);
        Vec3 translation = Vec3(0.0), translation_offset = Vec3(0.0);
        Vec3 rotation = Vec3(0.0), rotation_offet = Vec3(0.0);

        Transformation() = default;

        Mat4 GetTransform() const;
    };

    struct UniqueShader
    {
        Ref<Shader> shader;
        operator Ref<Shader>() const
        {
            return shader;
        }
    };
}  // namespace DE