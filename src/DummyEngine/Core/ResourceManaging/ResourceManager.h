#pragma once

#include "DummyEngine/Core/Rendering/Renderer/CubeMap.h"
#include "DummyEngine/Core/Rendering/Renderer/Shader.h"
#include "DummyEngine/Core/ResourceManaging/Assets.h"
#include "DummyEngine/Utils/Base.h"

namespace DE {
    class ResourceManager : public Singleton<ResourceManager> {
        SINGLETON(ResourceManager)
    public:
        S_METHOD_DEF(bool, LoadShader, (UUID id));
        S_METHOD_DEF(bool, LoadRenderMesh, (UUID id));
        S_METHOD_DEF(bool, LoadCubeMap, (UUID id));

        S_METHOD_DEF(std::optional<Ref<Shader>>, GetShader, (UUID id));
        S_METHOD_DEF(std::optional<Ref<RenderMesh>>, GetRenderMesh, (UUID id));
        S_METHOD_DEF(std::optional<Ref<CubeMap>>, GetCubeMap, (UUID id));

        S_METHOD_DEF(bool, HasShader, (UUID id));
        S_METHOD_DEF(bool, HasRenderMesh, (UUID id));
        S_METHOD_DEF(bool, HasCubeMap, (UUID id));

        S_METHOD_DEF(bool, DeleteShader, (UUID id));
        S_METHOD_DEF(bool, DeleteRenderMesh, (UUID id));
        S_METHOD_DEF(bool, DeleteCubeMap, (UUID id));

        S_METHOD_DEF(Unit, Clear, ());

    private:
        std::unordered_map<UUID, Ref<Shader>>     m_Shaders;
        std::unordered_map<UUID, Ref<RenderMesh>> m_RenderMeshes;
        std::unordered_map<UUID, Ref<CubeMap>>    m_CubeMaps;
    };
}  // namespace DE