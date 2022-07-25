#pragma once

#include <assimp/scene.h>
#include <assimp/Importer.hpp>

#include "Core/Rendering/Renderer/RenderStructs.h"

namespace DE {
namespace fs = std::filesystem;

class ModelLoader {
private:
    enum ColorType { none, diffuse, specular, ambient };
    size_t m_MeshesAmount;
    size_t m_VerticesAmount;
    size_t m_NodesAmount;
    size_t m_CurrentMeshId;
    std::unordered_map<std::string, TextureData> m_PathToTextureData;

    Path m_CurrentDirectory;
    RenderModelData* m_CurrentData;

    Assimp::Importer m_Importer;
    ModelLoader();

    static ModelLoader& Get();

    void ILoadModel(const Path& path, RenderModelData& data);
    void IProcessNode(aiNode* node, const aiScene* scene);
    void IProcessMesh(aiMesh* mesh, const aiScene* scene);
    TextureData ILoadMaterialTexture(aiMaterial* mat, aiTextureType type);
    Vec3 IGetmaterialColor(aiMaterial* mat, ColorType type);
    void IReadModelProperties(aiNode* node, const aiScene* scene);

public:
    static void LoadModel(const Path& path, RenderModelData& data);
};
}  // namespace DE