#pragma once

#include "Addition/Base.h"
#include "Core/Scene/Scene.h"
#include "Core/ECS/Entity.hpp"
#include "Core/ECS/ECSStorage.hpp"
#include "Core/Rendering/Renderer/Shader.h"
#include "Core/Rendering/Renderer/FrameBuffer.h"
#include "Core/Rendering/Renderer/UniformBuffer.h"

namespace DE
{
    class SceneRenderData
    {
    public:
        SceneRenderData(Scene* scene);

        void Render();
        void UpdateLights();
        void UpdateVP();

        void SetVPEntity(const Entity& entity);

        void RequestShader(UUID shader_id);
        void AddVPEntity(const Entity& entity);
        void SetCamera(const Entity& camera);

        Ref<RenderMeshInstance> GetRenderMeshInstance(UUID mesh_id, UUID shader_ids);

    private:
        void CreateInstancedMesh(UUID mesh_id, UUID shader_id);

        std::unordered_map<uint64_t, Ref<Shader>>                                                     m_Shaders;
        std::unordered_map<std::pair<UUID, UUID>, Pair<Ref<RenderMesh>, Ref<Shader>>, std::pair_hash> m_InstancedMeshes;
        std::unordered_map<EntityId, uint32_t>                                                        m_EntityToVPIndex;
        Ref<UniformBuffer>                                                                            m_Lights;
        Ref<UniformBuffer>                                                                            m_VP;
        Scene*                                                                                        m_Scene;
        Entity                                                                                        m_Camera;
    };
}  // namespace DE