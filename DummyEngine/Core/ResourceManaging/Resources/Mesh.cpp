//
// Created by balanda on 10/18/2023.
//

#include "Mesh.hpp"

#include <utility>

namespace DE {
    void SubMesh::Prepare() {
        DE_ASSERT(false, "No implementation yet");
        Resource::Prepare();
    }
    void SubMesh::Load() {
        m_RenderSubMesh = CreateRef<RenderSubMesh>();
        m_RenderSubMesh->FillData(m_Vertices, m_Indices, m_Material);
        Resource::Load();
    }
    void SubMesh::Unload() {
        m_RenderSubMesh->Remove();
        m_RenderSubMesh = nullptr;
        Resource::Unload();
    }
    void SubMesh::Remove() {
        m_Indices.resize(0);
        m_Vertices.resize(0);
        m_Indices.shrink_to_fit();
        m_Vertices.shrink_to_fit();
        m_Material = nullptr;
        Resource::Remove();
    }

    SubMesh::SubMesh(std::vector<Vertex3D>& vertices, std::vector<U32>& indices, Ref<Material> mat) :
        m_Vertices(vertices), m_Indices(indices), m_Material(std::move(mat)) {
        Resource::Prepare();
    }

    void Mesh::Prepare() {

        Resource::Prepare();
    }
    void Mesh::Load() {
        Resource::Load();
    }
    void Mesh::Unload() {
        Resource::Unload();
    }
    void Mesh::Remove() {
        Resource::Remove();
    }
}  // namespace DE