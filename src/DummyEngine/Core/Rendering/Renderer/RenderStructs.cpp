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

    void RenderSubMesh::FillData(const RenderSubMeshData& data)
    {
        BufferLayout layout({BufferElementType::Float3, BufferElementType::Float3, BufferElementType::Float2});

        vertex_array = VertexArray::Create();

        Ref<VertexBuffer> vertex_buffer = VertexBuffer::Create(layout, data.vertices.size(), &data.vertices[0]);
        Ref<IndexBuffer> index_buffer = IndexBuffer::Create(&data.indices[0], data.indices.size());

        material.ambient_color = data.material.ambient_color;
        material.specular_color = data.material.specular_color;
        material.diffuse_color = data.material.diffuse_color;
        material.shininess = data.material.shininess;

        material.specular_map = SetupTexture(data.material.specular_map);
        material.diffuse_map = SetupTexture(data.material.diffuse_map);

        vertex_array->AddVertexBuffer(vertex_buffer);
        vertex_array->SetIndexBuffer(index_buffer);
    }

    RenderMesh::RenderMesh(Ref<RenderMeshData> data)
    {
        FillData(data);
    }
    void RenderMesh::FillData(Ref<RenderMeshData> data)
    {
        meshes.resize(data->meshes.size());
        for (size_t i = 0; i < data->meshes.size(); ++i)
        {
            meshes[i].FillData(data->meshes[i]);
        }
    }

}  // namespace DE