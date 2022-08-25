#pragma once

#include "Addition/Base.h"
#include "Core/Rendering/Renderer/Shader.h"

namespace DE
{
    struct IdComponent
    {
        UUID id;

        IdComponent() = default;
        IdComponent(UUID uuid);
        IdComponent(const IdComponent&) = default;

        operator uint64_t() const
        {
            return id;
        }
    };

    struct TagComponent
    {
        std::string tag;

        TagComponent() = default;
        TagComponent(const std::string& tag);

        operator std::string() const
        {
            return tag;
        }
    };
    struct TransformComponent
    {
        Vec3 scale = Vec3(1.0), scale_offset = Vec3(1.0);
        Vec3 translation = Vec3(0.0), translation_offset = Vec3(0.0);
        Vec3 rotation = Vec3(0.0), rotation_offet = Vec3(0.0);

        TransformComponent() = default;

        Mat4 GetTransform() const;
    };
    struct ShaderComponent
    {
        UUID id;
        Ref<Shader> shader;
    };
    struct RenderMeshComponent
    {
        UUID id;
        Ref<RenderMeshInstance> mesh_instance = nullptr;
    };
}  // namespace DE