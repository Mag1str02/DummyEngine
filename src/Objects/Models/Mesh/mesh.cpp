#include "mesh.h"

Mesh &Mesh::operator+=(const Mesh &other)
{
    for (size_t i = 0; i < other.vertices.size(); ++i)
    {       
        vertices.push_back(other.vertices[i]);
    }
    size_t sz = indices.size();
    for(size_t i = 0; i < other.indices.size(); ++i){
        indices.push_back(other.indices[i] + sz);
    }
    return *this;
}