#pragma once

#include "DummyEngine/Core/ResourceManaging/Assets.h"
#include "DummyEngine/Utils/Debug/Logger.h"

#include <assimp/scene.h>
#include <assimp/Importer.hpp>

namespace DummyEngine {
    namespace fs = std::filesystem;

    class ModelLoader {
        LOG_AUTHOR(ModelLoader)
    public:
        static Ref<RenderMeshData> Load(const RenderMeshAsset::LoadingProperties& props);

    private:
        enum ColorType {
            None = 0,
            Albedo,
            Diffuse,
            Specular,
            Ambient,
            ORM,
            Emission,
        };

        struct LoaderState {
            size_t                             VerticesAmount;
            size_t                             NodesAmount;
            size_t                             CurrentMeshID;
            size_t                             MeshesAmount;
            Path                               CurrentDirectory;
            RenderMeshAsset::LoadingProperties Props;

            Ref<RenderMeshData>                        CurrentData;
            std::unordered_map<Path, Ref<TextureData>> ModelTextures;
            Assimp::Importer                           Importer;
        };

        static LoaderState gState;

        static void LoadBone(Bone& bone, aiNodeAnim* node);
        static void ReadWeights(aiMesh* mesh);
        static void ReadBones(Animation& animation, const aiAnimation* anim);
        static void ReadAnimationNode(Animation::Node& node, const aiNode* src);
        static void ReadAnimation(Animation& animation, const aiScene* scene);

        static MaterialData     LoadMaterial(aiMaterial* mat);
        static void             ProcessNode(aiNode* node, const aiScene* scene);
        static void             ProcessMesh(aiMesh* mesh, const aiScene* scene);
        static Vec3             GetColor(aiMaterial* mat, ColorType type);
        static Ref<TextureData> GetTexture(aiMaterial* mat, aiTextureType type);
        static void             ReadModelProperties(aiNode* node, const aiScene* scene);

    };  // namespace DummyEngine
}  // namespace DummyEngine