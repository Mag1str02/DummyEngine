#include "Core/ResourceManaging/ResourceManager.h"

namespace DE
{
    ResourceManager::Resources ResourceManager::m_Resources;

    template <> void ResourceManager::AddResource(const ShaderAsset& resource) { m_Resources.m_Shaders[resource.id] = Shader::Create(resource.parts); }
    template <> void ResourceManager::AddResource(const RenderMeshAsset& resource) { m_Resources.m_RenderMeshes[resource.id] = CreateRef<RenderMesh>(resource.mesh_data); }

    template <> Ref<Shader> ResourceManager::GetResource(UUID id)
    {
        DE_ASSERT(m_Resources.m_Shaders.find(id) != m_Resources.m_Shaders.end(), "Shader with UUID: " + std::to_string(id) + " does not exists.");
        return m_Resources.m_Shaders[id];
    }
    template <> Ref<RenderMesh> ResourceManager::GetResource(UUID id)
    {
        DE_ASSERT(m_Resources.m_RenderMeshes.find(id) != m_Resources.m_RenderMeshes.end(), "RenderMesh with UUID: " + std::to_string(id) + " does not exists.");
        return m_Resources.m_RenderMeshes[id];
    }

    template <> bool ResourceManager::HasResource<Shader>(UUID id) { return m_Resources.m_Shaders.find(id) != m_Resources.m_Shaders.end(); }
    template <> bool ResourceManager::HasResource<RenderMesh>(UUID id) { return m_Resources.m_RenderMeshes.find(id) != m_Resources.m_RenderMeshes.end(); }

}  // namespace DE