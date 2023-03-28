#pragma once

#include "DummyEngine/Core/Rendering/Renderer/CubeMap.h"
#include "DummyEngine/Core/Rendering/Renderer/Shader.h"
#include "DummyEngine/Utils/Base.h"

namespace DE {
    class IDComponent {
    public:
        IDComponent()                   = default;
        IDComponent(const IDComponent&) = default;
        IDComponent(UUID uuid) : m_ID(uuid) {}

        UUID        Get() const { return m_ID; }
        std::string Hex() const { return m_ID; }

        operator UUID() const { return m_ID; }

    private:
        UUID m_ID;
    };

    class TagComponent {
    public:
        TagComponent() = default;
        TagComponent(const std::string& tag) : m_Tag(tag) {}
        TagComponent(const TagComponent&) = default;

        const std::string& Get() const { return m_Tag; }

        operator std::string() const { return m_Tag; }

        bool operator==(const TagComponent& other) const { return m_Tag == other.m_Tag; }
        bool operator!=(const TagComponent& other) const { return m_Tag != other.m_Tag; }
        bool operator<(const TagComponent& other) const { return m_Tag < other.m_Tag; }
        bool operator>(const TagComponent& other) const { return m_Tag > other.m_Tag; }

    private:
        std::string m_Tag;
    };

    struct TransformComponent {
        Vec3 scale = Vec3(1.0), scale_offset = Vec3(1.0);
        Vec3 translation = Vec3(0.0), translation_offset = Vec3(0.0);
        Vec3 rotation = Vec3(0.0), rotation_offet = Vec3(0.0);

        TransformComponent() = default;

        Mat4 GetTransform() const;
    };
    struct ShaderComponent {
        UUID id;

        Ref<Shader> shader;
    };
    struct RenderMeshComponent {
        UUID id;

        Ref<RenderMeshInstance> mesh_instance = nullptr;
    };
    struct SkyBox {
        UUID id;

        Ref<CubeMap> map = nullptr;
    };
}  // namespace DE

namespace std {
    template <typename T> struct hash;

    template <> struct hash<DE::TagComponent> {
        std::size_t operator()(const DE::TagComponent& tag) const { return std::hash<std::string>()(tag.Get()); }
    };

}  // namespace std