#pragma once

#include "DummyEngine/Core/Animations/Animator.h"
#include "DummyEngine/Core/Rendering/Renderer/Shader.h"
#include "DummyEngine/Core/Rendering/Renderer/Texture.h"
#include "DummyEngine/Core/Rendering/Renderer/VertexArray.h"

namespace DummyEngine {

    struct Material {
        MaterialType Type             = MaterialType::None;
        float        Shininess        = 64;
        float        EmissionStrength = 0;

        Vec3 Albedo   = Vec3(1.0f);
        Vec3 ORM      = Vec3(1.0f);
        Vec3 Diffuse  = Vec3(1.0f);
        Vec3 Specular = Vec3(1.0f);
        Vec3 Ambient  = Vec3(1.0f);
        Vec3 Emission = Vec3(0.0f);

        Ref<Texture> AlbedoMap;
        Ref<Texture> NormalMap;
        Ref<Texture> ORMMap;
        Ref<Texture> DiffuseMap;
        Ref<Texture> SpecularMap;
        Ref<Texture> EmissionMap;

        void Apply(Ref<Shader> shader, const std::string& uniform_name = "u_Material") const;
        void FillData(const MaterialData& material);
    };

    struct RenderSubMesh {
        Material         Material;
        Ref<VertexArray> VertexArray;

        RenderSubMesh Copy() const;
        void          FillData(const RenderSubMeshData& data);
    };

    class RenderMesh {
    public:
        RenderMesh() {}
        explicit RenderMesh(Ref<RenderMeshData> data);

        Ref<RenderMesh>             Copy() const;
        std::vector<RenderSubMesh>& GetSubMeshes();
        void                        FillData(Ref<RenderMeshData> data);

        Ref<Animator> Animator;

    private:
        friend class Renderer;
        friend class RenderMeshInstance;

        std::vector<RenderSubMesh> sub_meshes_;
    };

}  // namespace DummyEngine