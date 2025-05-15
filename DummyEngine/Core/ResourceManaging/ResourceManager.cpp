#include "ResourceManager.h"

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

    S_METHOD_IMPL(TryFuture<Ref<RenderMeshData>>, LoadRenderMeshData, (UUID id), (id)) {
        if (render_meshes_.contains(id)) {
            LOG_WARNING("RenderMesh {} was not loaded because already loaded", id);
            return render_meshes_.at(id);
        }
        auto asset = AssetManager::GetRenderMeshAsset(id);
        if (!asset) {
            LOG_WARNING("RenderMesh {} was not loaded because does not exist in AssetManager", id);
            return Futures::Failure();
        }
        render_meshes_[id] = ModelLoader::Load(asset.value().LoadingProps) | Futures::Copy();

        LOG_INFO("RenderMesh {} was added", id);
        return render_meshes_[id];
    }
    S_METHOD_IMPL(TryFuture<Ref<TextureData>>, LoadTextureData, (UUID id), (id)) {
        if (textures_.contains(id)) {
            LOG_WARNING("Texture {} was not loaded because already loaded", id);
            return textures_.at(id);
        }
        auto asset = AssetManager::GetTextureAsset(id);
        if (!asset) {
            LOG_WARNING("Texture {} was not loaded because does not exist in AssetManager", id);
            return Futures::Failure();
        }

        textures_[id] = TextureLoader::Load(asset.value().LoadingProps) | Futures::Copy();
        LOG_INFO("Texture {} was added", id);
        return textures_.at(id);
    }
    S_METHOD_IMPL(TryFuture<Ref<Physics::ConvexHitbox>>, LoadHitBox, (UUID id), (id)) {
        if (hit_boxes_.contains(id)) {
            LOG_WARNING("HitBox {} was not loaded because already loaded", id);
            return hit_boxes_.at(id);
        }
        hit_boxes_[id] =              //
            LoadRenderMeshData(id) |  //
            Futures::MapOk([](Ref<RenderMeshData>&& mesh) {
                std::vector<Vec3> vertices;
                for (const auto& submesh : mesh->Meshes) {
                    for (const auto& vert : submesh.Vertices) {
                        vertices.push_back(vert.Position);
                    }
                }
                auto hitbox = CreateRef<Physics::ConvexHitbox>();
                hitbox->Build(vertices);
                return hitbox;
            }) |  //
            Futures::Copy();

        LOG_INFO("Hitbox {} was added", id);
        return hit_boxes_.at(id).Copy();
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
    S_METHOD_IMPL(Result<Ref<RenderMeshData>>, GetRenderMeshData, (UUID id), (id)) {
        if (render_meshes_.contains(id)) {
            return render_meshes_[id].Copy() | Futures::Get();
        }
        return Results::Failure();
    }
    S_METHOD_IMPL(std::optional<Ref<CubeMap>>, GetCubeMap, (UUID id), (id)) {
        if (cube_maps_.contains(id)) {
            return cube_maps_[id];
        }
        return {};
    }
    S_METHOD_IMPL(Result<Ref<TextureData>>, GetTextureData, (UUID id), (id)) {
        if (textures_.contains(id)) {
            return textures_[id].Copy() | Futures::Get();
        }
        return Results::Failure();
    }
    S_METHOD_IMPL(Result<Ref<Physics::ConvexHitbox>>, GetHitBox, (UUID id), (id)) {
        if (hit_boxes_.contains(id)) {
            return hit_boxes_[id].Copy() | Futures::Get();
        }
        return Results::Failure();
    }

    S_METHOD_IMPL(bool, HasShader, (UUID id), (id)) {
        return shaders_.contains(id);
    }
    S_METHOD_IMPL(bool, HasRenderMeshData, (UUID id), (id)) {
        return render_meshes_.contains(id);
    }
    S_METHOD_IMPL(bool, HasCubeMap, (UUID id), (id)) {
        return cube_maps_.contains(id);
    }
    S_METHOD_IMPL(bool, HasTextureData, (UUID id), (id)) {
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
    S_METHOD_IMPL(bool, DeleteRenderMeshData, (UUID id), (id)) {
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
    S_METHOD_IMPL(bool, DeleteTextureData, (UUID id), (id)) {
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