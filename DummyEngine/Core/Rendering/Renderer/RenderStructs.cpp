#include "DummyEngine/Core/Rendering/Renderer/RenderStructs.h"

#include "DummyEngine/Core/Rendering/Renderer/Renderer.h"

namespace DE {

    void Material::Apply(Ref<Shader> shader, const std::string& uniform_name) const {
        switch (type) {
            case MaterialType::PBR: {
                if (albedo_map) {
                    albedo_map->Bind(1);
                } else {
                    Renderer::GetTexture(Renderer::Textures::White)->Bind(1);
                }
                if (normal_map) {
                    normal_map->Bind(2);
                } else {
                    Renderer::GetTexture(Renderer::Textures::Normal)->Bind(2);
                }
                if (orm_map) {
                    orm_map->Bind(3);
                } else {
                    Renderer::GetTexture(Renderer::Textures::White)->Bind(3);
                }
                shader->SetInt(uniform_name + ".m_AlbedoMap", 1);
                shader->SetInt(uniform_name + ".m_NormalMap", 2);
                shader->SetInt(uniform_name + ".m_ORMMap", 3);
                shader->SetFloat3(uniform_name + ".m_Ambient", ambient);
                shader->SetFloat3(uniform_name + ".m_Albedo", albedo);
                shader->SetFloat3(uniform_name + ".m_ORM", orm);
                break;
            }
            case MaterialType::Phong: {
                if (diffuse_map) {
                    diffuse_map->Bind(1);
                } else {
                    Renderer::GetTexture(Renderer::Textures::White)->Bind(1);
                }
                if (normal_map) {
                    normal_map->Bind(2);
                } else {
                    Renderer::GetTexture(Renderer::Textures::Normal)->Bind(2);
                }
                if (specular_map) {
                    specular_map->Bind(3);
                } else {
                    Renderer::GetTexture(Renderer::Textures::White)->Bind(3);
                }

                shader->SetInt(uniform_name + ".m_DiffuseMap", 1);
                shader->SetInt(uniform_name + ".m_NormalMap", 2);
                shader->SetInt(uniform_name + ".m_SpecularMap", 3);
                shader->SetFloat3(uniform_name + ".m_Ambient", ambient);
                shader->SetFloat3(uniform_name + ".m_Diffuse", diffuse);
                shader->SetFloat3(uniform_name + ".m_Specular", specular);
                shader->SetFloat(uniform_name + ".m_Shininess", shininess);
                break;
            }
            case MaterialType::None: {
                if (diffuse_map) {
                    diffuse_map->Bind(1);
                } else {
                    Renderer::GetTexture(Renderer::Textures::White)->Bind(1);
                }
                if (normal_map) {
                    normal_map->Bind(2);
                } else {
                    Renderer::GetTexture(Renderer::Textures::Normal)->Bind(2);
                }
                if (specular_map) {
                    specular_map->Bind(3);
                } else {
                    Renderer::GetTexture(Renderer::Textures::White)->Bind(3);
                }
                if (albedo_map) {
                    albedo_map->Bind(4);
                } else {
                    Renderer::GetTexture(Renderer::Textures::White)->Bind(4);
                }
                if (orm_map) {
                    orm_map->Bind(5);
                } else {
                    Renderer::GetTexture(Renderer::Textures::White)->Bind(5);
                }
                shader->SetInt(uniform_name + ".m_DiffuseMap", 1);
                shader->SetInt(uniform_name + ".m_NormalMap", 2);
                shader->SetInt(uniform_name + ".m_SpecularMap", 3);
                shader->SetInt(uniform_name + ".m_AlbedoMap", 4);
                shader->SetInt(uniform_name + ".m_ORMMap", 5);
                shader->SetFloat3(uniform_name + ".m_Albedo", albedo);
                shader->SetFloat3(uniform_name + ".m_ORM", orm);
                shader->SetFloat3(uniform_name + ".m_Ambient", ambient);
                shader->SetFloat3(uniform_name + ".m_Diffuse", diffuse);
                shader->SetFloat3(uniform_name + ".m_Specular", specular);
                shader->SetFloat(uniform_name + ".m_Shininess", shininess);
                break;
            }
            default: DE_ASSERT(false, "Unsupported material type"); break;
        }
    }
    Ref<Texture> SetupTexture(Ref<TextureData> texture_data) {
        if (texture_data) {
            return Texture::Create(*texture_data);
        }
        return nullptr;
    }
    void Material::FillData(const MaterialData& material) {
        type      = material.type;
        shininess = material.shininess;

        ambient  = material.ambient;
        albedo   = material.albedo;
        diffuse  = material.diffuse;
        specular = material.specular;
        orm      = material.orm;

        albedo_map   = SetupTexture(material.albedo_map);
        normal_map   = SetupTexture(material.normal_map);
        orm_map      = SetupTexture(material.orm_map);
        diffuse_map  = SetupTexture(material.diffuse_map);
        specular_map = SetupTexture(material.specular_map);
    }

    void RenderSubMesh::FillData(const RenderSubMeshData& data) {
        BufferLayout layout({BufferElementType::Float3,
                             BufferElementType::Float3,
                             BufferElementType::Float3,
                             BufferElementType::Float2,
                             BufferElementType::Int4,
                             BufferElementType::Int4,
                             BufferElementType::Float4,
                             BufferElementType::Float4});

        vertex_array = VertexArray::Create();

        Ref<VertexBuffer> vertex_buffer = VertexBuffer::Create(layout, data.vertices.size(), &data.vertices[0]);
        Ref<IndexBuffer>  index_buffer  = IndexBuffer::Create(&data.indices[0], data.indices.size());
        material.FillData(data.material);

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
        res->p_Animator = p_Animator;
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
        if (data->animation) {
            p_Animator = CreateRef<Animator>(data->animation);
        }
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