#include "ResourceManager.h"

#include "DummyEngine/Core/Application/Concurrency.h"
#include "DummyEngine/Core/ResourceManaging/AssetManager.h"
#include "DummyEngine/ToolBox/Loaders/ModelLoader.h"
#include "DummyEngine/ToolBox/Loaders/TextureLoader.h"

namespace DummyEngine {

    SINGLETON_BASE(ResourceManager);
    S_INITIALIZE() {
        return Unit();
    }
    S_TERMINATE() {
        return Unit();
    }

    S_METHOD_IMPL(TryFuture<Ref<RenderMesh>>, LoadRenderMesh, (UUID id), (id)) {
        if (render_meshes_.contains(id)) {
            LOG_WARNING("RenderMesh {} was not loaded because already loaded", id);
            return Futures::Failure();
        }
        auto asset = AssetManager::GetRenderMeshAsset(id);
        if (!asset) {
            LOG_WARNING("RenderMesh {} was not loaded because does not exist in AssetManager", id);
            return Futures::Failure();
        }
        render_meshes_[id] = ModelLoader::Load(asset.value().LoadingProps) |                                            //
                             Futures::Via(Concurrency::GetEngineMainScheduler()) |                                      //
                             Futures::MapOk([](Ref<RenderMeshData>&& data) { return CreateRef<RenderMesh>(data); }) |  //
                             Futures::Copy();

        LOG_INFO("RenderMesh {} was added", id);
        return render_meshes_[id];
    }
    S_METHOD_IMPL(TryFuture<Ref<Texture>>, LoadTexture, (UUID id), (id)) {
        if (textures_.contains(id)) {
            LOG_WARNING("Texture {} was not loaded because already loaded", id);
            return Futures::Failure();
        }
        auto asset = AssetManager::GetTextureAsset(id);
        if (!asset) {
            LOG_WARNING("Texture {} was not loaded because does not exist in AssetManager", id);
            return Futures::Failure();
        }
        auto texture = TextureLoader::Load(asset.value().LoadingProps) |                                 //
                       Futures::Via(Concurrency::GetEngineMainScheduler()) |                             //
                       Futures::MapOk([](Ref<TextureData>&& data) { return Texture::Create(*data); }) |  //
                       Futures::Copy();

        textures_[id] = texture;
        LOG_INFO("Texture {} was added", id);
        return texture;
    }
    S_METHOD_IMPL(bool, LoadShader, (UUID id), (id)) {
        if (shaders_.contains(id)) {
            LOG_WARNING("Shader {} was not loaded because already loaded", id);
            return false;
        }
        auto asset = AssetManager::GetShaderAsset(id);
        if (!asset) {
            LOG_WARNING("Shader {} was not loaded because does not exist in AssetManager", id);
            return false;
        }
        shaders_[id] = Shader::Create(asset.value().Parts);
        LOG_INFO("Shader {} was added", id);
        return true;
    }
    S_METHOD_IMPL(bool, LoadHitBox, (UUID id), (id)) {
        if (hit_boxes_.contains(id)) {
            LOG_WARNING("HitBox {} was not loaded because already loaded", id);
            return false;
        }
        auto asset = AssetManager::GetRenderMeshAsset(id);
        if (!asset) {
            LOG_WARNING("Hitbox {} was not loaded because does not exist in AssetManager", id);
            return false;
        }
        auto mesh_result = ModelLoader::Load(asset.value().LoadingProps) | Futures::Get();
        if (mesh_result) {
            return false;
        }
        const auto&       mesh = mesh_result.value();
        std::vector<Vec3> vertices;
        for (const auto& submesh : mesh->Meshes) {
            for (const auto& vert : submesh.Vertices) {
                vertices.push_back(vert.Position);
            }
        }
        hit_boxes_.insert({id, CreateRef<Physics::ConvexHitbox>()});
        hit_boxes_[id]->Build(vertices);
        LOG_INFO("Hitbox {} was added", id);
        return true;
    }
    S_METHOD_IMPL(bool, LoadCubeMap, (UUID id), (id)) {
        if (cube_maps_.contains(id)) {
            LOG_WARNING("CubeMap {} was not loaded because already loaded", id);
            return false;
        }
        auto asset = AssetManager::GetTextureAsset(id);
        if (!asset) {
            LOG_WARNING("CubeMap {} was not loaded because does not exist in AssetManager", id);
            return false;
        }
        auto texture   = TextureLoader::Load(asset.value().LoadingProps) | Futures::Get();
        cube_maps_[id] = CubeMap::Create(*texture);
        LOG_INFO("CubeMap {} was added", id);
        return true;
    }

    S_METHOD_IMPL(std::optional<Ref<Shader>>, GetShader, (UUID id), (id)) {
        if (shaders_.contains(id)) {
            return shaders_[id];
        }
        return {};
    }
    S_METHOD_IMPL(Result<Ref<RenderMesh>>, GetRenderMesh, (UUID id), (id)) {
        if (render_meshes_.contains(id)) {
            return render_meshes_[id].Copy() | Futures::Get();
        }
        return {};
    }
    S_METHOD_IMPL(std::optional<Ref<CubeMap>>, GetCubeMap, (UUID id), (id)) {
        if (cube_maps_.contains(id)) {
            return cube_maps_[id];
        }
        return {};
    }
    S_METHOD_IMPL(Result<Ref<Texture>>, GetTexture, (UUID id), (id)) {
        if (textures_.contains(id)) {
            return textures_[id].Copy() | Futures::Get();
        }
        return {};
    }
    S_METHOD_IMPL(std::optional<Ref<Physics::ConvexHitbox>>, GetHitBox, (UUID id), (id)) {
        if (hit_boxes_.contains(id)) {
            return hit_boxes_[id];
        }
        return {};
    }

    S_METHOD_IMPL(bool, HasShader, (UUID id), (id)) {
        return shaders_.contains(id);
    }
    S_METHOD_IMPL(bool, HasRenderMesh, (UUID id), (id)) {
        return render_meshes_.contains(id);
    }
    S_METHOD_IMPL(bool, HasCubeMap, (UUID id), (id)) {
        return cube_maps_.contains(id);
    }
    S_METHOD_IMPL(bool, HasTexture, (UUID id), (id)) {
        return textures_.contains(id);
    }
    S_METHOD_IMPL(bool, HasHitBox, (UUID id), (id)) {
        return hit_boxes_.contains(id);
    }

    S_METHOD_IMPL(bool, DeleteShader, (UUID id), (id)) {
        if (shaders_.contains(id)) {
            shaders_.erase(id);
            LOG_INFO("Shader {} was deleted", id);
            return true;
        }
        return false;
    }
    S_METHOD_IMPL(bool, DeleteRenderMesh, (UUID id), (id)) {
        if (render_meshes_.contains(id)) {
            render_meshes_.erase(id);
            LOG_INFO("RenderMesh {} was deleted", id);
            return true;
        }
        return false;
    }
    S_METHOD_IMPL(bool, DeleteCubeMap, (UUID id), (id)) {
        if (cube_maps_.contains(id)) {
            cube_maps_.erase(id);
            LOG_INFO("CubeMap {} was deleted", id);
            return true;
        }
        return false;
    }
    S_METHOD_IMPL(bool, DeleteHitBox, (UUID id), (id)) {
        if (hit_boxes_.contains(id)) {
            hit_boxes_.erase(id);
            LOG_INFO("Hitbox {} was deleted", id);
            return true;
        }
        return false;
    }
    S_METHOD_IMPL(bool, DeleteTexture, (UUID id), (id)) {
        if (textures_.contains(id)) {
            textures_.erase(id);
            LOG_INFO("Texture {} was deleted", id);
            return true;
        }
        return false;
    }
    S_METHOD_IMPL(Unit, Clear, (), ()) {
        shaders_.clear();
        render_meshes_.clear();
        cube_maps_.clear();
        hit_boxes_.clear();
        textures_.clear();
        LOG_INFO("Cleared all resources");
        return Unit();
    }

}  // namespace DummyEngine