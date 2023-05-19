#include "DummyEngine/Core/Rendering/Renderer/RenderStructs.h"

#include "DummyEngine/Core/Rendering/Renderer/Renderer.h"

namespace DE {
    Ref<Texture> SetupTexture(Ref<TextureData> texture_data) {
        if (texture_data) {
            return Texture::Create(*texture_data);
        }
        return nullptr;
    }

    void RenderSubMesh::FillData(const RenderSubMeshData& data) {
        BufferLayout layout({BufferElementType::Float3, BufferElementType::Float3, BufferElementType::Float3, BufferElementType::Float2});

        vertex_array = VertexArray::Create();

        Ref<VertexBuffer> vertex_buffer = VertexBuffer::Create(layout, data.vertices.size(), &data.vertices[0]);
        Ref<IndexBuffer>  index_buffer  = IndexBuffer::Create(&data.indices[0], data.indices.size());

        material.ambient_color  = data.material.ambient_color;
        material.specular_color = data.material.specular_color;
        material.diffuse_color  = data.material.diffuse_color;
        material.shininess      = data.material.shininess;

        material.specular_map = SetupTexture(data.material.specular_map);
        material.diffuse_map  = SetupTexture(data.material.diffuse_map);
        material.normal_map   = SetupTexture(data.material.normal_map);

        vertex_array->AddVertexBuffer(vertex_buffer);
        vertex_array->SetIndexBuffer(index_buffer);
    }
    RenderSubMesh RenderSubMesh::Copy() const {
        RenderSubMesh res;
        res.material     = material;
        res.vertex_array = vertex_array->Copy();
        return res;
    }

    RenderMeshInstance::RenderMeshInstance(Ref<RenderMesh> mesh) {
        Bind(mesh);
    }
    RenderMeshInstance::~RenderMeshInstance() {
        UnBind();
    }
    void RenderMeshInstance::Bind(Ref<RenderMesh> mesh) {
        if (m_Mesh) {
            UnBind();
        }
        DE_ASSERT(mesh->m_InstanceBuffer, "Attaching RenderMeshInstance to RenderMesh without instance buffer");

        m_Index = mesh->m_Instances.size();
        m_Mesh  = mesh;
        m_Mesh->m_Instances.push_back(this);
    }
    void RenderMeshInstance::UnBind() {
        if (!m_Mesh) {
            return;
        }
        m_Mesh->m_Instances[m_Index]          = m_Mesh->m_Instances.back();
        m_Mesh->m_InstanceBuffer->at(m_Index) = m_Mesh->m_InstanceBuffer->at(m_Mesh->m_Instances.size() - 1);
        m_Mesh->m_Instances[m_Index]->m_Index = m_Index;
        m_Mesh->m_Instances.pop_back();
        m_Mesh = nullptr;
    }
    Ref<RenderMesh> RenderMeshInstance::GetMesh() {
        return m_Mesh;
    }

    RenderMesh::RenderMesh(Ref<RenderMeshData> data) : m_InstanceBuffer(nullptr) {
        FillData(data);
    }
    Ref<RenderMesh> RenderMesh::Copy() const {
        Ref<RenderMesh> res = CreateRef<RenderMesh>();
        res->m_SubMeshes.resize(m_SubMeshes.size());
        for (size_t i = 0; i < m_SubMeshes.size(); ++i) {
            res->m_SubMeshes[i] = m_SubMeshes[i].Copy();
        }
        return res;
    }
    std::vector<RenderSubMesh>& RenderMesh::GetSubMeshes() {
        return m_SubMeshes;
    }
    void RenderMesh::SetInstanceBuffer(const BufferLayout& layout, U32 size) {
        m_InstanceBuffer = VertexBuffer::Create(layout, size, BufferUsage::Dynamic);
        for (auto& sub_mesh : m_SubMeshes) {
            sub_mesh.vertex_array->AddVertexBuffer(m_InstanceBuffer);
        }
    }
    void RenderMesh::FillData(Ref<RenderMeshData> data) {
        m_SubMeshes.resize(data->meshes.size());
        for (size_t i = 0; i < data->meshes.size(); ++i) {
            m_SubMeshes[i].FillData(data->meshes[i]);
            if (m_InstanceBuffer) {
                m_SubMeshes[i].vertex_array->AddVertexBuffer(m_InstanceBuffer);
            }
        }
    }
    void RenderMesh::UpdateInstanceBuffer() {
        m_InstanceBuffer->PushData();
    }
}  // namespace DE