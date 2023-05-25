#pragma once

#include "DummyEngine/Core/Animations/Animator.h"
#include "DummyEngine/Core/Rendering/Renderer/Shader.h"
#include "DummyEngine/Core/Rendering/Renderer/Texture.h"
#include "DummyEngine/Core/Rendering/Renderer/VertexArray.h"
#include "DummyEngine/Utils/Base.h"

namespace DE {

    struct Material {
        MaterialType type = MaterialType::None;
        float        shininess;

        Vec3 albedo   = Vec3(1.0f);
        Vec3 orm      = Vec3(1.0f);
        Vec3 diffuse  = Vec3(1.0f);
        Vec3 specular = Vec3(1.0f);
        Vec3 ambient  = Vec3(1.0f);

        Ref<Texture> albedo_map;
        Ref<Texture> normal_map;
        Ref<Texture> orm_map;
        Ref<Texture> diffuse_map;
        Ref<Texture> specular_map;

        void Apply(Ref<Shader> shader, const std::string& uniform_name = "u_Material") const;
        void FillData(const MaterialData& material);
    };

    struct RenderSubMesh {
        Material         material;
        Ref<VertexArray> vertex_array;

        RenderSubMesh Copy() const;
        void          FillData(const RenderSubMeshData& data);
    };

    class RenderMesh;
    class RenderMeshInstance {
    public:
        RenderMeshInstance() : m_Index(0), m_Mesh(nullptr) {}
        RenderMeshInstance(Ref<RenderMesh> mesh);
        ~RenderMeshInstance();

        void Bind(Ref<RenderMesh> mesh);
        void UnBind();

        template <typename Type> Type& at(U32 index);
        Ref<RenderMesh>                GetMesh();

    private:
        U32             m_Index;
        Ref<RenderMesh> m_Mesh;
    };
    class RenderMesh {
    public:
        RenderMesh() : m_InstanceBuffer(nullptr) {}
        RenderMesh(Ref<RenderMeshData> data);
        Ref<RenderMesh>             Copy() const;
        std::vector<RenderSubMesh>& GetSubMeshes();

        void          UpdateInstanceBuffer();
        void          SetInstanceBuffer(const BufferLayout& layout, U32 size);
        void          FillData(Ref<RenderMeshData> data);
        Ref<Animator> p_Animator;

    private:
        friend class Renderer;
        friend class RenderMeshInstance;

        std::vector<RenderSubMesh>       m_SubMeshes;
        std::vector<RenderMeshInstance*> m_Instances;
        Ref<VertexBuffer>                m_InstanceBuffer;
    };
    template <typename Type> Type& RenderMeshInstance::at(U32 index) {
        return m_Mesh->m_InstanceBuffer->at(m_Index).Get<Type>(index);
    }

}  // namespace DE