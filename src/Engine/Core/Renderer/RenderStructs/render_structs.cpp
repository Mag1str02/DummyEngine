#include "render_structs.h"

#include <iostream>

namespace DE {

void RenderMesh::FillData(const RenderMeshData& data) {
    vertex_array.Bind();

    vertex_buffer.AllocateStorage(data.vertices.size() * sizeof(Vertex3D), &(data.vertices[0]));
    vertex_buffer.SetFloatAttribute(0, 3, 0, 8);
    vertex_buffer.SetFloatAttribute(1, 3, 3, 8);
    vertex_buffer.SetFloatAttribute(2, 2, 6, 8);
    element_buffer.AllocateStorage(data.indices);
    indices_amount = data.indices.size();

    material.ambient_color = data.material.ambient_color;
    material.specular_color = data.material.specular_color;
    material.diffuse_color = data.material.diffuse_color;
    material.shininess = data.material.shininess;

    material.specular_map.SetData(data.material.specular_map);
    material.diffuse_map.SetData(data.material.diffuse_map);
    material.normal_map.SetData(data.material.normal_map);

    vertex_array.UnBind();
}

void RenderModel::FillData(const RenderModelData& data) {
    render_meshes.resize(data.meshes.size());
    for (size_t i = 0; i < data.meshes.size(); ++i) {
        render_meshes[i].FillData(data.meshes[i]);
    }
}

}  // namespace DE