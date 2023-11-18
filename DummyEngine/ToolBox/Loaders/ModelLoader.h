#pragma once

#include <assimp/scene.h>

#include <assimp/Importer.hpp>

#include "DummyEngine/Core/ResourceManaging/Assets.h"
#include "DummyEngine/Core/ResourceManaging/Resources/Mesh.hpp"

namespace DE {
    namespace fs = std::filesystem;

    class ModelLoader {
    public:
        static Ref<RenderMeshData> Load(const RenderMeshAsset::LoadingProperties& props);

    private:
        enum ColorType { None = 0, Albedo, Diffuse, Specular, Ambient, ORM, Emission };

        struct LoaderState {
            size_t                             m_VerticesAmount;
            size_t                             m_NodesAmount;
            size_t                             m_CurrentMeshId;
            size_t                             m_MeshesAmount;
            Path                               m_CurrentDirectory;
            RenderMeshAsset::LoadingProperties m_Props;

            std::vector<Ref<SubMesh>> m_SubMeshes;
            Ref<Animation>            m_Animation;

            //            Ref<RenderMeshData>                        m_CurrentData; // TODO: remove
            std::unordered_map<Path, Ref<TextureResource>> m_ModelTextures;
            Assimp::Importer                           m_Importer;
        };

        static LoaderState m_State;

        static void LoadBone(Bone& bone, aiNodeAnim* node);
        static void ReadWeights(aiMesh* mesh);
        static void ReadBones(Animation& animation, const aiAnimation* anim);
        static void ReadAnimationNode(Animation::Node& node, const aiNode* src);
        static void ReadAnimation(Animation& animation, const aiScene* scene);

        static Ref<Material> LoadMaterial(aiMaterial* mat);
        static void          ProcessNode(aiNode* node, const aiScene* scene);
        static void          ProcessMesh(aiMesh* mesh, const aiScene* scene);
        static Vec3          GetColor(aiMaterial* mat, ColorType type);
        static Ref<TextureResource>  GetTexture(aiMaterial* mat, aiTextureType type);
        static void          ReadModelProperties(aiNode* node, const aiScene* scene);

    };  // namespace DE
}  // namespace DE