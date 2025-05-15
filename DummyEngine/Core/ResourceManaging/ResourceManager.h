#pragma once

#include "DummyEngine/Core/Physics/ConvexCollider.hpp"
#include "DummyEngine/Core/Rendering/Renderer/CubeMap.h"
#include "DummyEngine/Core/Rendering/Renderer/RenderStructs.h"
#include "DummyEngine/Core/Rendering/Renderer/Shader.h"

namespace DummyEngine {

    class ResourceManager : public Singleton<ResourceManager> {
        LOG_AUTHOR(ResourceManager)
        SINGLETON(ResourceManager)
    public:
        S_METHOD_DEF(TryFuture<Ref<TextureData>>, LoadTextureData, (UUID id));
        S_METHOD_DEF(TryFuture<Ref<RenderMeshData>>, LoadRenderMeshData, (UUID id));

        S_METHOD_DEF(bool, LoadShader, (UUID id));
        S_METHOD_DEF(bool, LoadCubeMap, (UUID id));
        S_METHOD_DEF(bool, LoadHitBox, (UUID id));

        S_METHOD_DEF(Result<Ref<TextureData>>, GetTextureData, (UUID id));
        S_METHOD_DEF(Result<Ref<RenderMeshData>>, GetRenderMeshData, (UUID id));
        S_METHOD_DEF(std::optional<Ref<Shader>>, GetShader, (UUID id));
        S_METHOD_DEF(std::optional<Ref<CubeMap>>, GetCubeMap, (UUID id));
        S_METHOD_DEF(std::optional<Ref<Physics::ConvexHitbox>>, GetHitBox, (UUID id));

        S_METHOD_DEF(bool, HasTextureData, (UUID id));
        S_METHOD_DEF(bool, HasRenderMeshData, (UUID id));
        S_METHOD_DEF(bool, HasShader, (UUID id));
        S_METHOD_DEF(bool, HasCubeMap, (UUID id));
        S_METHOD_DEF(bool, HasHitBox, (UUID id));

        S_METHOD_DEF(bool, DeleteTextureData, (UUID id));
        S_METHOD_DEF(bool, DeleteRenderMeshData, (UUID id));
        S_METHOD_DEF(bool, DeleteShader, (UUID id));
        S_METHOD_DEF(bool, DeleteCubeMap, (UUID id));
        S_METHOD_DEF(bool, DeleteHitBox, (UUID id));

        S_METHOD_DEF(Unit, Clear, ());

    private:
        std::unordered_map<UUID, CopyTryFuture<Ref<RenderMeshData>>> render_meshes_;
        std::unordered_map<UUID, CopyTryFuture<Ref<TextureData>>>    textures_;

        std::unordered_map<UUID, Ref<Shader>>                shaders_;
        std::unordered_map<UUID, Ref<CubeMap>>               cube_maps_;
        std::unordered_map<UUID, Ref<Physics::ConvexHitbox>> hit_boxes_;
    };

}  // namespace DummyEngine