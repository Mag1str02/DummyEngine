#pragma once

#include "DummyEngine/Core/Animations/Animator.h"
#include "DummyEngine/Core/Rendering/Renderer/Shader.h"
#include "DummyEngine/Core/Rendering/Renderer/Texture.h"
#include "DummyEngine/Core/Rendering/Renderer/VertexArray.h"
#include "DummyEngine/Utils/Base.h"

namespace DE {

    struct Material {
        MaterialType type              = MaterialType::None;
        float        shininess         = 64;
        float        emission_strength = 0;

        Vec3 albedo   = Vec3(1.0f);
        Vec3 orm      = Vec3(1.0f);
        Vec3 diffuse  = Vec3(1.0f);
        Vec3 specular = Vec3(1.0f);
        Vec3 ambient  = Vec3(1.0f);
        Vec3 emission = Vec3(0.0f);

        Ref<Texture> albedo_map;
        Ref<Texture> normal_map;
        Ref<Texture> orm_map;
        Ref<Texture> diffuse_map;
        Ref<Texture> specular_map;
        Ref<Texture> emission_map;

        void Apply(Ref<Shader> shader, const std::string& uniform_name = "u_Material") const;
        void FillData(const MaterialData& material);
    };

    struct RenderSubMesh {
        Material         material;
        Ref<VertexArray> vertex_array;

        RenderSubMesh Copy() const;
        void          FillData(const RenderSubMeshData& data);
        void          FillData(const std::vector<Vertex3D>& vertices, const std::vector<U32>& indices, const Ref<Material>& mat);
        void          Remove();
    };

    class RenderMesh {
    public:
        RenderMesh() {}
        RenderMesh(Ref<RenderMeshData> data);
        Ref<RenderMesh>             Copy() const;
        std::vector<RenderSubMesh>& GetSubMeshes();

        void          FillData(Ref<RenderMeshData> data);
        Ref<Animator> p_Animator;

    private:
        friend class Renderer;
        friend class RenderMeshInstance;

        std::vector<RenderSubMesh> m_SubMeshes;
    };
}  // namespace DE