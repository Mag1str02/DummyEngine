#pragma once

#include "Addition/Base.h"
#include "Core/Rendering/Renderer/Texture.h"
#include "Core/Rendering/Renderer/VertexArray.h"

namespace DE
{
    struct Material
    {
        Vec3 ambient_color;
        Vec3 diffuse_color;
        Vec3 specular_color;
        float shininess;
        Ref<Texture> specular_map;
        Ref<Texture> diffuse_map;
    };

    struct RenderSubMesh
    {
        Material material;
        Ref<VertexArray> vertex_array;

        RenderSubMesh Copy() const;
        void FillData(const RenderSubMeshData& data);
    };

    class RenderMesh;
    class RenderMeshInstance
    {
    public:
        RenderMeshInstance() : m_Index(0), m_Mesh(nullptr) {}
        RenderMeshInstance(Ref<RenderMesh> mesh);
        ~RenderMeshInstance();

        void Bind(Ref<RenderMesh> mesh);
        void UnBind();

        template <typename Type> Type& at(uint32_t index);

    private:
        uint32_t m_Index;
        Ref<RenderMesh> m_Mesh;
    };
    class RenderMesh
    {
    public:
        RenderMesh() : m_InstanceBuffer(nullptr) {}
        RenderMesh(Ref<RenderMeshData> data);
        Ref<RenderMesh> Copy() const;

        void UpdateInstanceBuffer();
        void SetInstanceBuffer(const BufferLayout& layout, uint32_t size);
        void FillData(Ref<RenderMeshData> data);

    private:
        friend class Renderer;
        friend class RenderMeshInstance;

        std::vector<RenderSubMesh> m_SubMeshes;
        std::vector<RenderMeshInstance*> m_Instances;
        Ref<VertexBuffer> m_InstanceBuffer;
    };
    template <typename Type> Type& RenderMeshInstance::at(uint32_t index)
    {
        return m_Mesh->m_InstanceBuffer->at(m_Index).Get<Type>(index);
    }

}  // namespace DE