#pragma once

#include "DummyEngine/Core/Rendering/Renderer/CubeMap.h"
#include "DummyEngine/Core/Rendering/Renderer/RenderStructs.h"
#include "DummyEngine/Core/Rendering/Renderer/Shader.h"
#include "DummyEngine/Core/Rendering/Renderer/SkyBox.h"
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

    struct TagComponent {
        TagComponent() = default;
        TagComponent(const std::string& tag) : tag(tag) {}
        operator std::string() const { return tag; }

        bool operator==(const TagComponent& other) const { return tag == other.tag; }
        bool operator!=(const TagComponent& other) const { return tag != other.tag; }
        bool operator<(const TagComponent& other) const { return tag < other.tag; }
        bool operator>(const TagComponent& other) const { return tag > other.tag; }

        std::string tag;
    };

    struct TransformComponent {
        Vec3 scale = Vec3(1.0), scale_offset = Vec3(1.0);
        Vec3 translation = Vec3(0.0), translation_offset = Vec3(0.0);
        Vec3 rotation = Vec3(0.0), rotation_offet = Vec3(0.0);

        TransformComponent() = default;

        Mat4 GetTransform() const;
        Mat4 GetTranslation() const;
        Mat4 GetRotation() const;
        Mat4 GetScale() const;
    };
    struct ShaderComponent {
        UUID id;

        Ref<Shader> shader;
        Shader*     operator->() { return shader.get(); }
    };
    struct RenderMeshComponent {
        UUID id;

        Ref<RenderMeshInstance> mesh_instance = nullptr;
        RenderMeshInstance*     operator->() { return mesh_instance.get(); }
    };

    struct SkyBoxComponent {
        enum class TexType {
            CubeMap = 0,
            Equirectangular,
        };
        TexType type;
        UUID    id;

        Ref<SkyBox> map;
        SkyBox*     operator->() { return map.get(); }
    };

    template <typename T> struct hash;

    template <> struct hash<DE::TagComponent> {
        std::size_t operator()(const DE::TagComponent& tag) const { return std::hash<std::string>()(tag); }
    };

}  // namespace DE