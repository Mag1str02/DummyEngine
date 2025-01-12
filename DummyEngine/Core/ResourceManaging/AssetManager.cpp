#include "DummyEngine/Core/ResourceManaging/AssetManager.h"

namespace DummyEngine {

    SINGLETON_BASE(AssetManager);
    S_INITIALIZE() {
        return Unit();
    }
    S_TERMINATE() {
        return Unit();
    }

    S_METHOD_IMPL(bool, AddScriptAsset, (const ScriptAsset& asset), (asset)) {
        if (scripts_.contains(asset.ID)) {
            return false;
        }
        scripts_[asset.ID] = asset;
        LOG_INFO("Added ScriptAsset({}|{})", asset.Name, asset.ID);
        return true;
    }
    S_METHOD_IMPL(bool, AddTextureAsset, (const TextureAsset& asset), (asset)) {
        if (textures_.contains(asset.ID)) {
            return false;
        }
        textures_[asset.ID] = asset;
        LOG_INFO("Added TextureAsset({}|{})", asset.Name, asset.ID);
        return true;
    }
    S_METHOD_IMPL(bool, AddShaderAsset, (const ShaderAsset& asset), (asset)) {
        if (shaders_.contains(asset.ID)) {
            return false;
        }
        shaders_[asset.ID] = asset;
        LOG_INFO("Added ShaderAsset({}|{})", asset.Name, asset.ID);
        return true;
    }
    S_METHOD_IMPL(bool, AddRenderMeshAsset, (const RenderMeshAsset& asset), (asset)) {
        if (render_meshes_.contains(asset.ID)) {
            return false;
        }
        render_meshes_[asset.ID] = asset;
        LOG_INFO("Added RenderMeshAsset({}|{})", asset.Name, asset.ID);
        return true;
    }

    S_METHOD_IMPL(std::optional<ScriptAsset>, GetScriptAsset, (UUID id), (id)) {
        if (!scripts_.contains(id)) {
            return {};
        }
        return scripts_[id];
    }
    S_METHOD_IMPL(std::optional<TextureAsset>, GetTextureAsset, (UUID id), (id)) {
        if (!textures_.contains(id)) {
            return {};
        }
        return textures_[id];
    }
    S_METHOD_IMPL(std::optional<ShaderAsset>, GetShaderAsset, (UUID id), (id)) {
        if (!shaders_.contains(id)) {
            return {};
        }
        return shaders_[id];
    }
    S_METHOD_IMPL(std::optional<RenderMeshAsset>, GetRenderMeshAsset, (UUID id), (id)) {
        if (!render_meshes_.contains(id)) {
            return {};
        }
        return render_meshes_[id];
    }

    S_METHOD_IMPL(bool, RemoveScriptAsset, (UUID id), (id)) {
        if (scripts_.contains(id)) {
            scripts_.erase(id);
            LOG_INFO("Remove ScriptAsset({})", id);
            return true;
        }
        return false;
    }
    S_METHOD_IMPL(bool, RemoveTextureAsset, (UUID id), (id)) {
        if (textures_.contains(id)) {
            textures_.erase(id);
            LOG_INFO("Remove TextureAsset({})", id);
            return true;
        }
        return false;
    }
    S_METHOD_IMPL(bool, RemoveShaderAsset, (UUID id), (id)) {
        if (shaders_.contains(id)) {
            shaders_.erase(id);
            LOG_INFO("Remove ShaderAsset({})", id);
            return true;
        }
        return false;
    }
    S_METHOD_IMPL(bool, RemoveRenderMeshAsset, (UUID id), (id)) {
        if (render_meshes_.contains(id)) {
            render_meshes_.erase(id);
            LOG_INFO("Remove RenderMeshAsset({})", id);
            return true;
        }
        return false;
    }

    S_METHOD_IMPL(Unit, Clear, (), ()) {
        render_meshes_.clear();
        textures_.clear();
        shaders_.clear();
        scripts_.clear();
        return Unit();
    }

}  // namespace DummyEngine