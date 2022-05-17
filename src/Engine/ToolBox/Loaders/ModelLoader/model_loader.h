#pragma once

#include <assimp/scene.h>

#include <assimp/Importer.hpp>
#include <filesystem>

#include "../../../Core/Structs/Meshes/mesh.h"

namespace DE {
namespace fs = std::filesystem;

class ModelLoader {
private:
    size_t _meshes_amount;
    size_t _verices_amount;
    size_t _nodes_amount;
    size_t _current_mesh_id;
    std::string _model_name;
    fs::path _directory;
    std::vector<Mesh>* _current_buffer;

    Assimp::Importer _importer;
    ModelLoader();

    static ModelLoader& Get();

    void IReadModel(const fs::path& path, std::vector<Mesh>& meshes, const std::string& model_name);
    void IProcessNode(aiNode* node, const aiScene* scene);
    void IProcessMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<const Texture2D*> ILoadMaterialTextures(aiMaterial* mat, aiTextureType type, Texture2DType texture_type);
    void IReadModelProperties(aiNode* node, const aiScene* scene);

public:
    static void ReadModel(const fs::path& path, std::vector<Mesh>& meshes, const std::string& model_name);
};
}  // namespace DE