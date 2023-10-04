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
            LOG_WARNING("ResourceManager", "Shader (", id, ") was not loaded because already loaded");
            return false;
        }
        auto asset = AssetManager::GetShaderAsset(id);
        if (!asset) {
            LOG_WARNING("ResourceManager", "Shader (", id, ") was not loaded because does not exist in AssetManager");
            return false;
        }
        m_Shaders.insert(id, Shader::Create(asset.value().parts));
        LOG_INFO("ResourceManager", "Shader (", id, ") was added");
        return true;
    }
    S_METHOD_IMPL(bool, LoadRenderMesh, (UUID id), (id)) {
        if (m_RenderMeshes.contains(id)) {
            LOG_WARNING("ResourceManager", "RenderMesh (", id, ") was not loaded because already loaded");
            return false;
        }
        auto asset = AssetManager::GetRenderMeshAsset(id);
        if (!asset) {
            LOG_WARNING("ResourceManager", "RenderMesh (", id, ") was not loaded because does not exist in AssetManager");
            return false;
        }
        m_RenderMeshes.insert(id, CreateRef<RenderMesh>(ModelLoader::Load(asset.value().loading_props)));
        LOG_INFO("ResourceManager", "RenderMesh (", id, ") was added");
        return true;
    }
    S_METHOD_IMPL(bool, LoadHitBox, (UUID id), (id)) {
        if (m_HitBoxes.contains(id)) {
            LOG_WARNING("ResourceManager", "HitBox (", id, ") was not loaded because already loaded");
            return false;
        }
        auto asset = AssetManager::GetRenderMeshAsset(id);
        if (!asset) {
            LOG_WARNING("ResourceManager", "Hitbox (", id, ") was not loaded because does not exist in AssetManager");
            return false;
        }
        auto              mesh = ModelLoader::Load(asset.value().loading_props);
        std::vector<Vec3> vertices;
        for (const auto& submesh : mesh->meshes) {
            for (const auto& vert : submesh.vertices) {
                vertices.push_back(vert.position);
            }
        }
        auto hitbox = CreateRef<Physics::ConvexHitbox>();
        hitbox->Build(vertices);
        m_HitBoxes.insert(id, hitbox);
        LOG_INFO("ResourceManager", "Hitbox (", id, ") was added");
        return true;
    }
    S_METHOD_IMPL(bool, LoadCubeMap, (UUID id), (id)) {
        if (m_CubeMaps.contains(id)) {
            LOG_WARNING("ResourceManager", "CubeMap (", id, ") was not loaded because already loaded");
            return false;
        }
        auto asset = AssetManager::GetTextureAsset(id);
        if (!asset) {
            LOG_WARNING("ResourceManager", "CubeMap (", id, ") was not loaded because does not exist in AssetManager");
            return false;
        }
        m_CubeMaps.insert(id, CubeMap::Create(TextureLoader::Load(asset.value().loading_props)));
        LOG_INFO("ResourceManager", "CubeMap (", id, ") was added");
        return true;
    }
    S_METHOD_IMPL(bool, LoadTexture, (UUID id), (id)) {
        if (m_Textures.contains(id)) {
            LOG_WARNING("ResourceManager", "Texture (", id, ") was not loaded because already loaded");
            return false;
        }
        auto asset = AssetManager::GetTextureAsset(id);
        if (!asset) {
            LOG_WARNING("ResourceManager", "Texture (", id, ") was not loaded because does not exist in AssetManager");
            return false;
        }
        m_Textures.insert(id, Texture::Create(*TextureLoader::Load(asset.value().loading_props)));
        LOG_INFO("ResourceManager", "Texture (", id, ") was added");
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
            LOG_INFO("ResourceManager", "Shader (", id, ") was deleted");
            return true;
        }
        return false;
    }
    S_METHOD_IMPL(bool, DeleteRenderMesh, (UUID id), (id)) {
        if (m_RenderMeshes.contains(id)) {
            m_RenderMeshes.erase(id);
            LOG_INFO("ResourceManager", "RenderMesh (", id, ") was deleted");
            return true;
        }
        return false;
    }
    S_METHOD_IMPL(bool, DeleteCubeMap, (UUID id), (id)) {
        if (m_CubeMaps.contains(id)) {
            m_CubeMaps.erase(id);
            LOG_INFO("ResourceManager", "CubeMap (", id, ") was deleted");
            return true;
        }
        return false;
    }
    S_METHOD_IMPL(bool, DeleteHitBox, (UUID id), (id)) {
        if (m_HitBoxes.contains(id)) {
            m_HitBoxes.erase(id);
            LOG_INFO("ResourceManager", "Hitbox (", id, ") was deleted");
            return true;
        }
        return false;
    }
    S_METHOD_IMPL(bool, DeleteTexture, (UUID id), (id)) {
        if (m_Textures.contains(id)) {
            m_Textures.erase(id);
            LOG_INFO("ResourceManager", "Texture (", id, ") was deleted");
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
        LOG_INFO("ResourceManager", "Cleared all resources");
        return Unit();
    }
}  // namespace DE