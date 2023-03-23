#include "DummyEngine/Core/ResourceManaging/ResourceManager.h"

#include "DummyEngine/Core/ResourceManaging/AssetManager.h"
#include "DummyEngine/ToolBox/Loaders/ModelLoader.h"
#include "DummyEngine/ToolBox/Loaders/TextureLoader.h"

namespace DE {
    SINGLETON_BASE(ResourceManager);
    S_INITIALIZE() {
        return Unit();
    }
    S_TERMINATE() {
        return Unit();
    }

    S_METHOD_IMPL(bool, LoadShader, (UUID id), (id)) {
        if (m_Shaders.contains(id)) {
            return false;
        }
        auto asset = AssetManager::GetShaderAsset(id);
        if (!asset) {
            return false;
        }
        m_Shaders[id] = Shader::Create(asset.value().parts);
        return true;
    }
    S_METHOD_IMPL(bool, LoadRenderMesh, (UUID id), (id)) {
        if (m_RenderMeshes.contains(id)) {
            return false;
        }
        auto asset = AssetManager::GetRenderMeshAsset(id);
        if (!asset) {
            return false;
        }
        m_RenderMeshes[id] = CreateRef<RenderMesh>(ModelLoader::Load(asset.value().loading_props));
        return true;
    }
    S_METHOD_IMPL(bool, LoadCubeMap, (UUID id), (id)) {
        if (m_CubeMaps.contains(id)) {
            return false;
        }
        auto asset = AssetManager::GetTextureAsset(id);
        if (!asset) {
            return false;
        }
        m_CubeMaps[id] = CubeMap::Create(TextureLoader::Load(asset.value().loading_props));
        return true;
    }

    S_METHOD_IMPL(std::optional<Ref<Shader>>, GetShader, (UUID id), (id)) {
        if (m_Shaders.contains(id)) {
            return m_Shaders[id];
        }
        return {};
    }
    S_METHOD_IMPL(std::optional<Ref<RenderMesh>>, GetRenderMesh, (UUID id), (id)) {
        if (m_RenderMeshes.contains(id)) {
            return m_RenderMeshes[id];
        }
        return {};
    }
    S_METHOD_IMPL(std::optional<Ref<CubeMap>>, GetCubeMap, (UUID id), (id)) {
        if (m_CubeMaps.contains(id)) {
            return m_CubeMaps[id];
        }
        return {};
    }

    S_METHOD_IMPL(bool, HasShader, (UUID id), (id)) {
        return m_Shaders.contains(id);
    }
    S_METHOD_IMPL(bool, HasRenderMesh, (UUID id), (id)) {
        return m_RenderMeshes.contains(id);
    }
    S_METHOD_IMPL(bool, HasCubeMap, (UUID id), (id)) {
        return m_CubeMaps.contains(id);
    }

    S_METHOD_IMPL(bool, DeleteShader, (UUID id), (id)) {
        if (m_Shaders.contains(id)) {
            m_Shaders.erase(id);
            return true;
        }
        return false;
    }
    S_METHOD_IMPL(bool, DeleteRenderMesh, (UUID id), (id)) {
        if (m_RenderMeshes.contains(id)) {
            m_RenderMeshes.erase(id);
            return true;
        }
        return false;
    }
    S_METHOD_IMPL(bool, DeleteCubeMap, (UUID id), (id)) {
        if (m_CubeMaps.contains(id)) {
            m_CubeMaps.erase(id);
            return true;
        }
        return false;
    }
}  // namespace DE