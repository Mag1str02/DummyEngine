#pragma once

#include <assimp/scene.h>

#include <assimp/Importer.hpp>
#include <filesystem>

#include "../../../Core/Rendering/RenderData/render_data.h"

namespace DE {
namespace fs = std::filesystem;

class ModelLoader {
private:
    enum ColorType { none, diffuse, specular, ambient };
    size_t _meshes_amount;
    size_t _verices_amount;
    size_t _nodes_amount;
    size_t _current_mesh_id;
    std::unordered_map<std::string, Texture2DData> _path_to_texture_data;

    fs::path _directory;
    RenderModelData* _current_data;

    Assimp::Importer _importer;
    ModelLoader();

    static ModelLoader& Get();

    void ILoadModel(const fs::path& path, RenderModelData& data);
    void IProcessNode(aiNode* node, const aiScene* scene);
    void IProcessMesh(aiMesh* mesh, const aiScene* scene);
    Texture2DData ILoadMaterialTexture(aiMaterial* mat, aiTextureType type);
    glm::vec3 IGetmaterialColor(aiMaterial* mat, ColorType type);
    void IReadModelProperties(aiNode* node, const aiScene* scene);

public:
    static void LoadModel(const fs::path& path, RenderModelData& data);
};
}  // namespace DE