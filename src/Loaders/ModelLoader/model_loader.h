#pragma once

#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <assimp/Importer.hpp>
#include <filesystem>

#include "../../Objects/Models/Mesh/mesh.h"

namespace fs = std::filesystem;

class ModelLoader
{
private:
    size_t _meshes_amount;
    size_t _verices_amount;
    size_t _nodes_amount;
    size_t _current_mesh_id;
    std::string _directory;
    std::vector<Mesh> *_current_buffer;

    Assimp::Importer _importer;
    ModelLoader();

    static ModelLoader &Get();

    void IReadModel(const fs::path &path, std::vector<Mesh> &meshes);
    void IProcessNode(aiNode *node, const aiScene *scene);
    void IProcessMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<Texture> ILoadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
    void IReadModelProperties(aiNode *node, const aiScene *scene);

public:
    static void ReadModel(const fs::path &path, std::vector<Mesh> &meshes);
};