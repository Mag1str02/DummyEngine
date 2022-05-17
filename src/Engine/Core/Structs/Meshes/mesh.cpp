#include "mesh.h"
namespace DE {
Mesh& Mesh::operator+=(const Mesh& other) {
    size_t sz = vertices.size();
    for (size_t i = 0; i < other.vertices.size(); ++i) {
        vertices.push_back(other.vertices[i]);
    }
    for (size_t i = 0; i < other.indices.size(); ++i) {
        indices.push_back(other.indices[i] + sz);
    }
    return *this;
}
}  // namespace DE