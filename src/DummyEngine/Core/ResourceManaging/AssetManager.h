#pragma once

#include "Addition/Base.h"
#include "Core/ResourceManaging/Assets.h"

namespace DE
{
    class AssetManager
    {
    public:
        template <typename AssetType> static void AddAsset(const AssetType& asset);
        template <typename AssetType> static const AssetType& GetAsset(UUID id);
        template <typename AssetType> static std::vector<AssetType> GetAllAssets();

        void Clear();

    private:
        struct AssetManagerData
        {
            std::unordered_map<uint64_t, RenderMeshAsset> m_RenderMeshes;
            std::unordered_map<uint64_t, TextureAsset> m_Textures;
            std::unordered_map<uint64_t, ShaderAsset> m_Shaders;
            std::unordered_map<uint64_t, ScriptAsset> m_Scripts;
        };
        static AssetManagerData m_Data;
    };
}  // namespace DE