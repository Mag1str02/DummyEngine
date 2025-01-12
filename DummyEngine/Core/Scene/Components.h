#pragma once

#include "DummyEngine/Core/Rendering/Renderer/RenderStructs.h"
#include "DummyEngine/Core/Rendering/Renderer/Shader.h"
#include "DummyEngine/Core/Rendering/Renderer/SkyBox.h"
#include "DummyEngine/Utils/Types/UUID.h"

namespace DummyEngine {

    class IDComponent {
    public:
        IDComponent() = default;
        IDComponent(UUID uuid) : id_(uuid) {}  // NOLINT
        operator UUID() const { return id_; }  // NOLINT

        UUID        Get() const { return id_; }
        std::string Hex() const { return id_; }

    private:
        UUID id_;
    };

    struct TagComponent {
        TagComponent() = default;
        TagComponent(const std::string& tag) : Tag(tag) {}  // NOLINT
        operator std::string() const { return Tag; }        // NOLINT

        bool operator==(const TagComponent& other) const { return Tag == other.Tag; }
        bool operator!=(const TagComponent& other) const { return Tag != other.Tag; }
        bool operator<(const TagComponent& other) const { return Tag < other.Tag; }
        bool operator>(const TagComponent& other) const { return Tag > other.Tag; }

        std::string Tag;
    };

    struct TransformComponent {
        Vec3 Scale             = Vec3(1.0);
        Vec3 ScaleOffset       = Vec3(1.0);
        Vec3 Translation       = Vec3(0.0);
        Vec3 TranslationOffset = Vec3(0.0);
        Vec3 Rotation          = Vec3(0.0);
        Vec3 RotationOffet     = Vec3(0.0);

        TransformComponent() = default;

        Mat4 GetTransform() const;
        Mat4 GetTranslation() const;
        Mat4 GetRotation() const;
        Mat4 GetScale() const;
    };

    struct ShaderComponent {
        UUID        ID;
        Ref<Shader> Shader;

        class Shader* operator->() { return Shader.get(); }
    };

    struct RenderMeshComponent {
        UUID            ID;
        Ref<RenderMesh> Mesh = nullptr;

        RenderMesh* operator->() { return Mesh.get(); }
    };

    struct SkyBoxComponent {
        enum class TexType {
            CubeMap = 0,
            Equirectangular,
        };

        TexType     Type;
        UUID        ID;
        Ref<SkyBox> SkyBox;

        class SkyBox* operator->() { return SkyBox.get(); }
    };

}  // namespace DummyEngine

namespace std {

    template <> struct hash<DummyEngine::TagComponent> {
        std::size_t operator()(const DummyEngine::TagComponent& tag) const { return std::hash<std::string>()(tag); }
    };

}  // namespace std