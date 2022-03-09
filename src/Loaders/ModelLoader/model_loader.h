#pragma once

#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <assimp/Importer.hpp>
#include <filesystem>

#include "../../Objects/Models/Mesh/mesh.h"

namespace fs = std::filesystem;

class ModelLoader {
private:
    size_t _meshes_amount;
    size_t _verices_amount;
    size_t _nodes_amount;

    Assimp::Importer _importer;
    ModelLoader();

    static ModelLoader& Get();

    void IReadModel(const fs::path& path, std::vector<Mesh>& meshes);
    void IProcessNode(aiNode* node, const aiScene* scene);
    void IProcessMesh(aiMesh* mesh, const aiScene* scene);
    size_t ICountMeshes(aiNode* node){
        
    }

public:
};