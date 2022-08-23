#include "Core/ResourceManaging/AssetManager.h"

namespace DE
{
    AssetManager::AssetManagerData AssetManager::m_Data;

    void AssetManager::Clear()
    {
        m_Data.m_RenderMeshes.clear();
        m_Data.m_Textures.clear();
    }

    template <> void AssetManager::AddAsset<TextureAsset>(const TextureAsset& asset)
    {
        DE_ASSERT(m_Data.m_Textures.find(asset.id) == m_Data.m_Textures.end(),
                  "Texture asset with UUID: " + std::to_string(asset.id) + " already exists.");
        m_Data.m_Textures[asset.id] = asset;
    }
    template <> void AssetManager::AddAsset<RenderMeshAsset>(const RenderMeshAsset& asset)
    {
        DE_ASSERT(m_Data.m_RenderMeshes.find(asset.id) == m_Data.m_RenderMeshes.end(),
                  "RenderMesh asset with UUID: " + std::to_string(asset.id) + " already exists.");
        m_Data.m_RenderMeshes[asset.id] = asset;
    }
    template <> void AssetManager::AddAsset<ShaderAsset>(const ShaderAsset& asset)
    {
        DE_ASSERT(m_Data.m_Shaders.find(asset.id) == m_Data.m_Shaders.end(),
                  "RenderMesh asset with UUID: " + std::to_string(asset.id) + " already exists.");
        m_Data.m_Shaders[asset.id] = asset;
    }

    template <> const RenderMeshAsset& AssetManager::GetAsset<RenderMeshAsset>(UUID id)
    {
        DE_ASSERT(m_Data.m_RenderMeshes.find(id) != m_Data.m_RenderMeshes.end(),
                  "RenderMesh asset with UUID: " + std::to_string(id) + " does not exists.");
        return m_Data.m_RenderMeshes[id];
    }
    template <> const TextureAsset& AssetManager::GetAsset<TextureAsset>(UUID id)
    {
        DE_ASSERT(m_Data.m_Textures.find(id) != m_Data.m_Textures.end(), "Texture asset with UUID: " + std::to_string(id) + " does not exists.");
        return m_Data.m_Textures[id];
    }
    template <> const ShaderAsset& AssetManager::GetAsset<ShaderAsset>(UUID id)
    {
        DE_ASSERT(m_Data.m_Shaders.find(id) != m_Data.m_Shaders.end(), "RenderMesh asset with UUID: " + std::to_string(id) + " does not exists.");
        return m_Data.m_Shaders[id];
    }

    template <> std::vector<RenderMeshAsset> AssetManager::GetAllAssets()
    {
        std::vector<RenderMeshAsset> result;
        for (const auto& [id, asset] : m_Data.m_RenderMeshes)
        {
            result.push_back(asset);
        }
        return result;
    }
    template <> std::vector<ShaderAsset> AssetManager::GetAllAssets()
    {
        std::vector<ShaderAsset> result;
        for (const auto& [id, asset] : m_Data.m_Shaders)
        {
            result.push_back(asset);
        }
        return result;
    }
    template <> std::vector<TextureAsset> AssetManager::GetAllAssets()
    {
        std::vector<TextureAsset> result;
        for (const auto& [id, asset] : m_Data.m_Textures)
        {
            result.push_back(asset);
        }
        return result;
    }
}  // namespace DE