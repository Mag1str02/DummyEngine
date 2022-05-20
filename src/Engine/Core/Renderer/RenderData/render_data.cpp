#include "render_data.h"

namespace DE {
RenderMeshData& RenderMeshData::operator+=(const RenderMeshData& other) {
    size_t sz = vertices.size();
    for (size_t i = 0; i < other.vertices.size(); ++i) {
        vertices.push_back(other.vertices[i]);
    }
    for (size_t i = 0; i < other.indices.size(); ++i) {
        indices.push_back(other.indices[i] + sz);
    }
    return *this;
}
void RenderModelData::Compress() {
    std::vector<RenderMeshData> new_vec(1);
    new_vec.back() = meshes.front();
    for (size_t i = 1; i < meshes.size(); ++i) {
        new_vec.back() += meshes[i];
    }
    meshes = new_vec;
}
}  // namespace DE