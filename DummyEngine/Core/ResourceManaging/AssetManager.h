#pragma once

#include "DummyEngine/Core/ResourceManaging/Assets.h"
#include "DummyEngine/Utils/Debug/Logger.h"
#include "DummyEngine/Utils/Helpers/Singleton.h"

namespace DummyEngine {

    class AssetManager : public Singleton<AssetManager> {
        LOG_AUTHOR(AssetManager)
        SINGLETON(AssetManager)
    public:
        S_METHOD_DEF(bool, AddScriptAsset, (const ScriptAsset& asset));
        S_METHOD_DEF(bool, AddTextureAsset, (const TextureAsset& asset));
        S_METHOD_DEF(bool, AddShaderAsset, (const ShaderAsset& asset));
        S_METHOD_DEF(bool, AddRenderMeshAsset, (const RenderMeshAsset& asset));

        S_METHOD_DEF(std::optional<ScriptAsset>, GetScriptAsset, (UUID id));
        S_METHOD_DEF(std::optional<TextureAsset>, GetTextureAsset, (UUID id));
        S_METHOD_DEF(std::optional<ShaderAsset>, GetShaderAsset, (UUID id));
        S_METHOD_DEF(std::optional<RenderMeshAsset>, GetRenderMeshAsset, (UUID id));

        S_METHOD_DEF(bool, RemoveScriptAsset, (UUID id));
        S_METHOD_DEF(bool, RemoveTextureAsset, (UUID id));
        S_METHOD_DEF(bool, RemoveShaderAsset, (UUID id));
        S_METHOD_DEF(bool, RemoveRenderMeshAsset, (UUID id));

        S_METHOD_DEF(Unit, Clear, ());

    private:
        std::unordered_map<UUID, RenderMeshAsset> render_meshes_;
        std::unordered_map<UUID, TextureAsset>    textures_;
        std::unordered_map<UUID, ShaderAsset>     shaders_;
        std::unordered_map<UUID, ScriptAsset>     scripts_;
    };

}  // namespace DummyEngine