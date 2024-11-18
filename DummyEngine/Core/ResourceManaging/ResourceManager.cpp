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
            LOG_WARNING("Shader {} was not loaded because already loaded", id);
            return false;
        }
        auto asset = AssetManager::GetShaderAsset(id);
        if (!asset) {
            LOG_WARNING("Shader {} was not loaded because does not exist in AssetManager", id);
            return false;
        }
        m_Shaders[id] = Shader::Create(asset.value().parts);
        LOG_INFO("Shader {} was added", id);
        return true;
    }
    S_METHOD_IMPL(bool, LoadRenderMesh, (UUID id), (id)) {
        if (m_RenderMeshes.contains(id)) {
            LOG_WARNING("RenderMesh {} was not loaded because already loaded", id);
            return false;
        }
        auto asset = AssetManager::GetRenderMeshAsset(id);
        if (!asset) {
            LOG_WARNING("RenderMesh {} was not loaded because does not exist in AssetManager", id);
            return false;
        }
        auto model = ModelLoader::Load(asset.value().loading_props);
        if (model == nullptr) {
            return false;
        }
        m_RenderMeshes[id] = CreateRef<RenderMesh>(model);
        LOG_INFO("RenderMesh {} was added", id);
        return true;
    }
    S_METHOD_IMPL(bool, LoadHitBox, (UUID id), (id)) {
        if (m_HitBoxes.contains(id)) {
            LOG_WARNING("HitBox {} was not loaded because already loaded", id);
            return false;
        }
        auto asset = AssetManager::GetRenderMeshAsset(id);
        if (!asset) {
            LOG_WARNING("Hitbox {} was not loaded because does not exist in AssetManager", id);
            return false;
        }
        auto              mesh = ModelLoader::Load(asset.value().loading_props);
        if (mesh == nullptr) {
            return false;
        }
        std::vector<Vec3> vertices;
        for (const auto& submesh : mesh->meshes) {
            for (const auto& vert : submesh.vertices) {
                vertices.push_back(vert.position);
            }
        }
        m_HitBoxes.insert({id, CreateRef<Physics::ConvexHitbox>()});
        m_HitBoxes[id]->Build(vertices);
        LOG_INFO("Hitbox {} was added", id);
        return true;
    }
    S_METHOD_IMPL(bool, LoadCubeMap, (UUID id), (id)) {
        if (m_CubeMaps.contains(id)) {
            LOG_WARNING("CubeMap {} was not loaded because already loaded", id);
            return false;
        }
        auto asset = AssetManager::GetTextureAsset(id);
        if (!asset) {
            LOG_WARNING("CubeMap {} was not loaded because does not exist in AssetManager", id);
            return false;
        }
        m_CubeMaps[id] = CubeMap::Create(TextureLoader::Load(asset.value().loading_props));
        LOG_INFO("CubeMap {} was added", id);
        return true;
    }
    S_METHOD_IMPL(bool, LoadTexture, (UUID id), (id)) {
        if (m_Textures.contains(id)) {
            LOG_WARNING("Texture {} was not loaded because already loaded", id);
            return false;
        }
        auto asset = AssetManager::GetTextureAsset(id);
        if (!asset) {
            LOG_WARNING("Texture {} was not loaded because does not exist in AssetManager", id);
            return false;
        }
        m_Textures[id] = Texture::Create(*TextureLoader::Load(asset.value().loading_props));
        LOG_INFO("Texture {} was added", id);
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
    S_METHOD_IMPL(std::optional<Ref<Texture>>, GetTexture, (UUID id), (id)) {
        if (m_Textures.contains(id)) {
            return m_Textures[id];
        }
        return {};
    }
    S_METHOD_IMPL(std::optional<Ref<Physics::ConvexHitbox>>, GetHitBox, (UUID id), (id)) {
        if (m_HitBoxes.contains(id)) {
            return m_HitBoxes[id];
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
    S_METHOD_IMPL(bool, HasTexture, (UUID id), (id)) {
        return m_Textures.contains(id);
    }
    S_METHOD_IMPL(bool, HasHitBox, (UUID id), (id)) {
        return m_HitBoxes.contains(id);
    }

    S_METHOD_IMPL(bool, DeleteShader, (UUID id), (id)) {
        if (m_Shaders.contains(id)) {
            m_Shaders.erase(id);
            LOG_INFO("Shader {} was deleted", id);
            return true;
        }
        return false;
    }
    S_METHOD_IMPL(bool, DeleteRenderMesh, (UUID id), (id)) {
        if (m_RenderMeshes.contains(id)) {
            m_RenderMeshes.erase(id);
            LOG_INFO("RenderMesh {} was deleted", id);
            return true;
        }
        return false;
    }
    S_METHOD_IMPL(bool, DeleteCubeMap, (UUID id), (id)) {
        if (m_CubeMaps.contains(id)) {
            m_CubeMaps.erase(id);
            LOG_INFO("CubeMap {} was deleted", id);
            return true;
        }
        return false;
    }
    S_METHOD_IMPL(bool, DeleteHitBox, (UUID id), (id)) {
        if (m_HitBoxes.contains(id)) {
            m_HitBoxes.erase(id);
            LOG_INFO("Hitbox {} was deleted", id);
            return true;
        }
        return false;
    }
    S_METHOD_IMPL(bool, DeleteTexture, (UUID id), (id)) {
        if (m_Textures.contains(id)) {
            m_Textures.erase(id);
            LOG_INFO("Texture {} was deleted", id);
            return true;
        }
        return false;
    }
    S_METHOD_IMPL(Unit, Clear, (), ()) {
        m_Shaders.clear();
        m_RenderMeshes.clear();
        m_CubeMaps.clear();
        m_HitBoxes.clear();
        m_Textures.clear();
        LOG_INFO("Cleared all resources");
        return Unit();
    }
}  // namespace DE