#pragma once

#include "Addition/Base.h"
#include "Core/Scene/Scene.h"
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

        Ref<RenderMeshInstance> GetRenderMeshInstance(UUID mesh_id, UUID shader_ids);

        void RequestShader(UUID shader_id);

    private:
        void CreateInstancedMesh(UUID mesh_id, UUID shader_id);

        std::unordered_map<uint64_t, Ref<Shader>>                                                     m_Shaders;
        std::unordered_map<std::pair<UUID, UUID>, Pair<Ref<RenderMesh>, Ref<Shader>>, std::pair_hash> m_InstancedMeshes;
        Ref<UniformBuffer>                                                                            m_Lights;
        Scene*                                                                                        m_Scene;
    };
}  // namespace DE