#pragma once

#include <assimp/scene.h>
#include <assimp/Importer.hpp>

#include "Core/Rendering/Renderer/RenderStructs.h"

namespace DE
{
    namespace fs = std::filesystem;

    class ModelLoader
    {
    public:
        static Ref<RenderModelData> LoadModel(const Path& path);

    private:
        enum ColorType
        {
            none,
            diffuse,
            specular,
            ambient
        };

        struct LoaderState
        {
            size_t m_VerticesAmount;
            size_t m_NodesAmount;
            size_t m_CurrentMeshId;
            size_t m_MeshesAmount;
            Path m_CurrentDirectory;

            Ref<RenderModelData> m_CurrentData;
            std::unordered_map<Path, Ref<RenderModelData>> m_ModelDataByPath;
            Assimp::Importer m_Importer;
        };

        static LoaderState m_State;

        static void ProcessNode(aiNode* node, const aiScene* scene);
        static void ProcessMesh(aiMesh* mesh, const aiScene* scene);
        static Vec3 GetColor(aiMaterial* mat, ColorType type);
        static Ref<TextureData> GetTexture(aiMaterial* mat, aiTextureType type);
        static void ReadModelProperties(aiNode* node, const aiScene* scene);
    };
}  // namespace DE