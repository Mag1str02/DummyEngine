#pragma once

#include "DummyEngine/Core/ECS/ECS.h"
#include "DummyEngine/Core/Rendering/Renderer/FrameBuffer.h"
#include "DummyEngine/Core/Rendering/Renderer/Shader.h"
#include "DummyEngine/Core/Rendering/Renderer/UniformBuffer.h"
#include "DummyEngine/Core/Scene/Scene.h"
#include "DummyEngine/Utils/Base.h"

namespace DE {
    class SceneRenderer {
    public:
        SceneRenderer(Scene* scene);

        void             Render(Entity camera);
        void             OnViewPortResize(U32 x, U32 y);
        Ref<FrameBuffer> GetFrameBuffer();

        void RequestShader(UUID shader_id);
        void AddVPEntity(const Entity& entity);

        Ref<RenderMeshInstance> GetRenderMeshInstance(UUID mesh_id, UUID shader_ids);

    private:
        void UpdateLights();
        void UpdateVP();

        void SetVPEntity(const Entity& entity);
        void CreateInstancedMesh(UUID mesh_id, UUID shader_id);

        Scene* m_Scene;

        Ref<UniformBuffer> m_Lights;
        Ref<UniformBuffer> m_VP;
        Ref<FrameBuffer>   m_FrameBuffer;

        std::unordered_map<UUID, Ref<Shader>>                                                   m_Shaders;
        std::unordered_map<std::pair<UUID, UUID>, Pair<Ref<RenderMesh>, Ref<Shader>>, PairHash> m_InstancedMeshes;
        std::unordered_map<Entity, U32>                                                         m_EntityToVPIndex;
    };
}  // namespace DE