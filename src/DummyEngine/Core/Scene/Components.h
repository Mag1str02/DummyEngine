#pragma once

#include "DummyEngine/Addition/Base.h"
#include "DummyEngine/Core/Rendering/Renderer/CubeMap.h"
#include "DummyEngine/Core/Rendering/Renderer/Shader.h"

namespace DE
{
    struct IdComponent
    {
        UUID id;

        IdComponent() = default;
        IdComponent(UUID uuid);
        IdComponent(const IdComponent&) = default;

                    operator uint64_t() const { return id; }
        std::string Hex() const { return id.Hex(); }
    };

    struct TagComponent
    {
        std::string tag;

        TagComponent() = default;
        TagComponent(const std::string& tag);

        operator std::string() const { return tag; }
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
    struct SkyBox
    {
        UUID id;

        Ref<CubeMap> map = nullptr;
    };

    class ScriptInstance;

    struct ScriptComponent
    {
        UUID id;

        Ref<ScriptInstance> instance = nullptr;
    };
}  // namespace DE