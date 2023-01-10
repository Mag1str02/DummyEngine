#pragma once

#include "DummyEngine/Addition/Base.h"
#include "DummyEngine/Core/ResourceManaging/Assets.h"
#include "DummyEngine/Core/Rendering/Renderer/Shader.h"
#include "DummyEngine/Core/Rendering/Renderer/CubeMap.h"

namespace DE
{
    class ResourceManager
    {
    public:
        template <typename ResourceType, typename InputResource> static void AddResource(const InputResource& resource);
        template <typename ResourceType> static Ref<ResourceType> GetResource(UUID id);
        template <typename ResourceType> static bool HasResource(UUID id);

    private:
        struct Resources
        {
            std::unordered_map<uint64_t, Ref<Shader>> m_Shaders;
            std::unordered_map<uint64_t, Ref<RenderMesh>> m_RenderMeshes;
            std::unordered_map<uint64_t, Ref<CubeMap>> m_CubeMaps;
        };
        static Resources m_Resources;
    };

}  // namespace DE