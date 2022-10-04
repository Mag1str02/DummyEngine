#pragma once

#include "Core/ECS/ECSStorage.hpp"
#include "Core/Objects/Cameras/FPSCamera.h"

namespace DE
{
    class Entity;
    class SceneRenderData;

    class Scene
    {
    public:
        Scene(const std::string& name = "Scene");

        Entity CreateHiddenEntity(const std::string& name = "Entity");
        Entity CreateEntity(const std::string& name = "Entity");
        Entity CreateEntityWithUUID(UUID uuid, const std::string& name = "Entity");
        Entity CloneEntity(const std::string& entity_to_clone, const std::string& new_name);

        Entity              GetByUUID(UUID uuid);
        Entity              GetByName(const std::string& name);
        Entity              GetCamera();
        std::string         GetName() const;
        std::vector<Entity> GetAllEntities();

        void OnUpdate(double dt);
        void OnViewPortResize(uint32_t x, uint32_t y);
        void UpdateScripts(double dt);
        void Render();

        template <typename System> void RegisterSystem() { m_Storage.RegisterSystem<System>(); }

    private:
        friend class SceneLoader;
        friend class SceneRenderData;
        friend class Entity;

        Entity CreateEmptyEntity();
        void   UpdateEmptyEntity(Entity entity);
        void   OnEntityDestroy(Entity entity);

        template <typename Component> void OnComponentAttach(Entity entity);

        std::string                               m_Name;
        ECSStorage                                m_Storage;
        Ref<SceneRenderData>                      m_RenderData;
        std::unordered_map<uint64_t, EntityId>    m_EntityByUUID;
        std::unordered_map<std::string, EntityId> m_EntityByName;
    };
}  // namespace DE