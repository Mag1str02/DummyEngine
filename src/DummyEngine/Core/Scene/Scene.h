#pragma once

#include "Core/ECS/ECSStorage.hpp"
#include "Core/Rendering/Renderer/Shader.h"
#include "Core/Rendering/Renderer/FrameBuffer.h"
#include "Core/Objects/Cameras/FPSCamera.h"

namespace DE
{
    class Entity;

    class Scene
    {
    public:
        Scene(const std::string& name = "Scene");

        Entity CreateEntity(const std::string& name = "Entity");
        Entity CreateEntityWithUUID(UUID uuid, const std::string& name = "Entity");
        Entity CloneEntity(const std::string& entity_to_clone, const std::string& new_name);

        Entity GetByUUID(UUID uuid);
        Entity GetByName(const std::string& name);
        FPSCamera& GetCamera();
        std::string GetName() const;
        std::vector<Entity> GetAllEntities();

        void OnUpdate(double dt);
        void OnViewPortResize(uint32_t x, uint32_t y);
        void Render();

        Entity operator[](const std::string& name);

        template <typename System> void RegisterSystem()
        {
            m_Storage.RegisterSystem<System>();
        }

    private:
        friend class SceneLoader;

        Entity CreateEmptyEntity();
        void UpdateEmptyEntity(Entity entity);
        void OnEntityDestroy(Entity entity);

        void LightPass();

        void RequestShader(UUID id);

        struct RenderData
        {
            std::unordered_map<uint64_t, Ref<Shader>> m_Shaders;
            std::unordered_map<std::pair<uint64_t, uint64_t>, Pair<Ref<RenderMesh>, Ref<Shader>>, std::pair_hash> m_InstancedMeshes;
        };

        std::string m_Name;
        ECSStorage m_Storage;
        RenderData m_RenderData;
        std::unordered_map<uint64_t, EntityId> m_EntityByUUID;
        std::unordered_map<std::string, EntityId> m_EntityByName;

        friend class Entity;
    };
}  // namespace DE