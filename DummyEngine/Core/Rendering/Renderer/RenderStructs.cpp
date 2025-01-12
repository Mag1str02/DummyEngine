#include "RenderStructs.h"

#include "DummyEngine/Core/Animations/Animator.h"
#include "DummyEngine/Core/Rendering/Renderer/Renderer.h"
#include "DummyEngine/Utils/Types/Types.h"

namespace DummyEngine {

    void Material::Apply(Ref<Shader> shader, const std::string& uniform_name) const {
        if (DiffuseMap) {
            DiffuseMap->Bind(1);
        } else {
            Renderer::GetTexture(Renderer::Textures::White)->Bind(1);
        }
        if (NormalMap) {
            NormalMap->Bind(2);
        } else {
            Renderer::GetTexture(Renderer::Textures::Normal)->Bind(2);
        }
        if (SpecularMap) {
            SpecularMap->Bind(3);
        } else {
            Renderer::GetTexture(Renderer::Textures::White)->Bind(3);
        }
        if (AlbedoMap) {
            AlbedoMap->Bind(4);
        } else {
            Renderer::GetTexture(Renderer::Textures::White)->Bind(4);
        }
        if (ORMMap) {
            ORMMap->Bind(5);
        } else {
            Renderer::GetTexture(Renderer::Textures::White)->Bind(5);
        }
        if (EmissionMap) {
            EmissionMap->Bind(6);
        } else {
            Renderer::GetTexture(Renderer::Textures::White)->Bind(6);
        }
        shader->SetInt(uniform_name + ".m_DiffuseMap", 1);
        shader->SetInt(uniform_name + ".m_NormalMap", 2);
        shader->SetInt(uniform_name + ".m_SpecularMap", 3);
        shader->SetInt(uniform_name + ".m_AlbedoMap", 4);
        shader->SetInt(uniform_name + ".m_ORMMap", 5);
        shader->SetInt(uniform_name + ".m_EmissionMap", 6);
        shader->SetFloat3(uniform_name + ".m_Albedo", Albedo);
        shader->SetFloat3(uniform_name + ".m_ORM", ORM);
        shader->SetFloat3(uniform_name + ".m_Ambient", Ambient);
        shader->SetFloat3(uniform_name + ".m_Diffuse", Diffuse);
        shader->SetFloat3(uniform_name + ".m_Specular", Specular);
        shader->SetFloat3(uniform_name + ".m_Emission", Emission);
        shader->SetFloat(uniform_name + ".m_Shininess", Shininess);
        shader->SetFloat(uniform_name + ".m_EmissionStrength", EmissionStrength);
    }
    Ref<Texture> SetupTexture(Ref<TextureData> texture_data) {
        if (texture_data) {
            return Texture::Create(*texture_data);
        }
        return nullptr;
    }
    void Material::FillData(const MaterialData& material) {
        Type      = material.Type;
        Shininess = material.Shininess;

        Emission = material.Emission;
        Ambient  = material.Ambient;
        Albedo   = material.Albedo;
        Diffuse  = material.Diffuse;
        Specular = material.Specular;
        ORM      = material.ORM;

        AlbedoMap   = SetupTexture(material.AlbedoMap);
        NormalMap   = SetupTexture(material.NormalMap);
        ORMMap      = SetupTexture(material.ORMMap);
        DiffuseMap  = SetupTexture(material.DiffuseMap);
        SpecularMap = SetupTexture(material.SpecularMap);
        EmissionMap = SetupTexture(material.EmissionMap);
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

        VertexArray = VertexArray::Create();

        Ref<VertexBuffer> vertex_buffer = VertexBuffer::Create(layout, data.Vertices.size(), &data.Vertices[0]);
        Ref<IndexBuffer>  index_buffer  = IndexBuffer::Create(&data.Indices[0], data.Indices.size());
        Material.FillData(data.Material);

        VertexArray->AddVertexBuffer(vertex_buffer);
        VertexArray->SetIndexBuffer(index_buffer);
    }
    RenderSubMesh RenderSubMesh::Copy() const {
        RenderSubMesh res;
        res.Material    = Material;
        res.VertexArray = VertexArray->Copy();
        return res;
    }
    RenderMesh::RenderMesh(Ref<RenderMeshData> data) {
        FillData(data);
    }
    Ref<RenderMesh> RenderMesh::Copy() const {
        Ref<RenderMesh> res = CreateRef<RenderMesh>();
        res->sub_meshes_.resize(sub_meshes_.size());
        res->Animator = Animator;
        for (size_t i = 0; i < sub_meshes_.size(); ++i) {
            res->sub_meshes_[i] = sub_meshes_[i].Copy();
        }
        return res;
    }
    std::vector<RenderSubMesh>& RenderMesh::GetSubMeshes() {
        return sub_meshes_;
    }
    void RenderMesh::FillData(Ref<RenderMeshData> data) {
        sub_meshes_.resize(data->Meshes.size());
        if (data->Animation) {
            Animator = CreateRef<class Animator>(data->Animation);
        }
        for (size_t i = 0; i < data->Meshes.size(); ++i) {
            sub_meshes_[i].FillData(data->Meshes[i]);
        }
    }
}  // namespace DummyEngine