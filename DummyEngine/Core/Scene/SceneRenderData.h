#pragma once

#include "DummyEngine/Utils/Base.h"
#include "DummyEngine/Core/Scene/Scene.h"
#include "DummyEngine/Core/ECS/ECS.h"
#include "DummyEngine/Core/Rendering/Renderer/Shader.h"
#include "DummyEngine/Core/Rendering/Renderer/FrameBuffer.h"
#include "DummyEngine/Core/Rendering/Renderer/UniformBuffer.h"

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
        std::unordered_map<Entity, uint32_t>                                                          m_EntityToVPIndex;
        Ref<UniformBuffer>                                                                            m_Lights;
        Ref<UniformBuffer>                                                                            m_VP;
        Scene*                                                                                        m_Scene;
        Entity                                                                                        m_Camera;
    };
}  // namespace DE