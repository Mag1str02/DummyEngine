#include "Core/Rendering/Renderer/RenderStructs.h"
#include "Core/Rendering/Renderer/Renderer.h"

namespace DE
{
    Ref<Texture> SetupTexture(Ref<TextureData> texture_data)
    {
        if (texture_data)
        {
            return Texture::Create(*texture_data);
        }
        else
        {
            return Renderer::GetDefaultTexture();
        }
    }

    RenderMeshData& RenderMeshData::operator+=(const RenderMeshData& other)
    {
        size_t sz = vertices.size();
        for (size_t i = 0; i < other.vertices.size(); ++i)
        {
            vertices.push_back(other.vertices[i]);
        }
        for (size_t i = 0; i < other.indices.size(); ++i)
        {
            indices.push_back(other.indices[i] + sz);
        }
        return *this;
    }
    void RenderModelData::Compress()
    {
        std::vector<RenderMeshData> new_vec(1);
        new_vec.back() = meshes.front();
        for (size_t i = 1; i < meshes.size(); ++i)
        {
            new_vec.back() += meshes[i];
        }
        meshes = new_vec;
    }
    void RenderMesh::FillData(RenderMeshData data)
    {
        vertex_array = VertexArray::Create();

        Ref<VertexBuffer> vertex_buffer =
            VertexBuffer::Create(&data.vertices[0], data.vertices.size() * sizeof(Vertex3D));
        Ref<IndexBuffer> index_buffer = IndexBuffer::Create(&data.indices[0], data.indices.size());

        BufferLayout layout({BufferElement(BufferElementType::Float3),
                             BufferElement(BufferElementType::Float3),
                             BufferElement(BufferElementType::Float2)});
        vertex_buffer->SetLayout(layout);

        material.ambient_color = data.material.ambient_color;
        material.specular_color = data.material.specular_color;
        material.diffuse_color = data.material.diffuse_color;
        material.shininess = data.material.shininess;

        material.specular_map = SetupTexture(data.material.specular_map);
        material.diffuse_map = SetupTexture(data.material.diffuse_map);
        material.normal_map = SetupTexture(data.material.normal_map);

        vertex_array->AddVertexBuffer(vertex_buffer);
        vertex_array->SetIndexBuffer(index_buffer);
    }
    void RenderModel::FillData(Ref<RenderModelData> data)
    {
        path = data->path;
        meshes.resize(data->meshes.size());
        for (size_t i = 0; i < data->meshes.size(); ++i)
        {
            meshes[i].FillData(data->meshes[i]);
        }
    }

}  // namespace DE