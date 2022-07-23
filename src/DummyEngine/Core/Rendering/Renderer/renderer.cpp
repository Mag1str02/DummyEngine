#include "DummyEngine/Core/Rendering/Renderer/renderer.h"

namespace DE
{

    void RenderMesh::FillData(const RenderMeshData& data)
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
        
        material.specular_map = Texture::Create(data.material.specular_map);
        material.diffuse_map = Texture::Create(data.material.diffuse_map);
        material.normal_map = Texture::Create(data.material.normal_map);
        
        vertex_array->AddVertexBuffer(vertex_buffer);
        vertex_array->SetIndexBuffer(index_buffer);
    }

    void RenderModel::FillData(const RenderModelData& data)
    {
        render_meshes.resize(data.meshes.size());
        for (size_t i = 0; i < data.meshes.size(); ++i)
        {
            render_meshes[i].FillData(data.meshes[i]);
        }
    }

    API Renderer::CurrentAPI()
    {
        return _render_api;
    }
    void Renderer::Init(API api)
    {
        _render_api = api;
    }
    void Renderer::Terminate()
    {
        _render_api = API::None;
    }
}  // namespace DE
