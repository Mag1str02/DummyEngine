#pragma once

#include "Addition/Base.h"
#include "Core/ResourceManaging/Assets.h"
#include "Core/Rendering/Renderer/Shader.h"

namespace DE
{
    class ResourceManager
    {
    public:
        template <typename ResourceType> static void AddResource(const ResourceType& resource);
        template <typename ResourceType> static Ref<ResourceType> GetResource(UUID id);
        template <typename ResourceType> static bool HasResource(UUID id);

    private:
        struct Resources
        {
            std::unordered_map<uint64_t, Ref<Shader>> m_Shaders;
            std::unordered_map<uint64_t, Ref<RenderMesh>> m_RenderMeshes;
        };
        static Resources m_Resources;
    };

}  // namespace DE