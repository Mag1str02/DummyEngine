#include "DummyEngine/Core/ResourceManaging/AssetManager.h"

namespace DE {
    SINGLETON_BASE(AssetManager);
    S_INITIALIZE() {
        return Unit();
    }
    S_TERMINATE() {
        return Unit();
    }

    S_METHOD_IMPL(bool, AddScriptAsset, (const ScriptAsset& asset), (asset)) {
        if (m_Scripts.contains(asset.id)) {
            return false;
        }
        m_Scripts[asset.id] = asset;
        LOG_INFO("AssetManager", "Added ScriptAsset(", asset.name, "|", asset.id.Hex(), ")");
        return true;
    }
    S_METHOD_IMPL(bool, AddTextureAsset, (const TextureAsset& asset), (asset)) {
        if (m_Textures.contains(asset.id)) {
            return false;
        }
        m_Textures[asset.id] = asset;
        LOG_INFO("AssetManager", "Added TextureAsset(", asset.name, "|", asset.id.Hex(), ")");
        return true;
    }
    S_METHOD_IMPL(bool, AddShaderAsset, (const ShaderAsset& asset), (asset)) {
        if (m_Shaders.contains(asset.id)) {
            return false;
        }
        m_Shaders[asset.id] = asset;
        LOG_INFO("AssetManager", "Added ShaderAsset(", asset.name, "|", asset.id.Hex(), ")");
        return true;
    }
    S_METHOD_IMPL(bool, AddRenderMeshAsset, (const RenderMeshAsset& asset), (asset)) {
        if (m_RenderMeshes.contains(asset.id)) {
            return false;
        }
        m_RenderMeshes[asset.id] = asset;
        LOG_INFO("AssetManager", "Added RenderMeshAsset(", asset.name, "|", asset.id.Hex(), ")");
        return true;
    }

    S_METHOD_IMPL(std::optional<ScriptAsset>, GetScriptAsset, (UUID id), (id)) {
        if (!m_Scripts.contains(id)) {
            return {};
        }
        return m_Scripts[id];
    }
    S_METHOD_IMPL(std::optional<TextureAsset>, GetTextureAsset, (UUID id), (id)) {
        if (!m_Textures.contains(id)) {
            return {};
        }
        return m_Textures[id];
    }
    S_METHOD_IMPL(std::optional<ShaderAsset>, GetShaderAsset, (UUID id), (id)) {
        if (!m_Shaders.contains(id)) {
            return {};
        }
        return m_Shaders[id];
    }
    S_METHOD_IMPL(std::optional<RenderMeshAsset>, GetRenderMeshAsset, (UUID id), (id)) {
        if (!m_RenderMeshes.contains(id)) {
            return {};
        }
        return m_RenderMeshes[id];
    }

    S_METHOD_IMPL(bool, RemoveScriptAsset, (UUID id), (id)) {
        if (m_Scripts.contains(id)) {
            m_Scripts.erase(id);
            LOG_INFO("AssetManager", "Remove ScriptAsset(", id.Hex(), ")");
            return true;
        }
        return false;
    }
    S_METHOD_IMPL(bool, RemoveTextureAsset, (UUID id), (id)) {
        if (m_Textures.contains(id)) {
            m_Textures.erase(id);
            LOG_INFO("AssetManager", "Remove TextureAsset(", id.Hex(), ")");
            return true;
        }
        return false;
    }
    S_METHOD_IMPL(bool, RemoveShaderAsset, (UUID id), (id)) {
        if (m_Shaders.contains(id)) {
            m_Shaders.erase(id);
            LOG_INFO("AssetManager", "Remove ShaderAsset(", id.Hex(), ")");
            return true;
        }
        return false;
    }
    S_METHOD_IMPL(bool, RemoveRenderMeshAsset, (UUID id), (id)) {
        if (m_RenderMeshes.contains(id)) {
            m_RenderMeshes.erase(id);
            LOG_INFO("AssetManager", "Remove RenderMeshAsset(", id.Hex(), ")");
            return true;
        }
        return false;
    }

    S_METHOD_IMPL(Unit, Clear, (), ()) {
        m_RenderMeshes.clear();
        m_Textures.clear();
        m_Shaders.clear();
        m_Scripts.clear();
        return Unit();
    }

}  // namespace DE