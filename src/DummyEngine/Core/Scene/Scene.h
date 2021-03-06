#pragma once

#include "Core/ECS/ECSStorage.hpp"
#include "Core/Objects/Cameras/FPSCamera.h"

namespace DE
{
    class Entity;

    class Scene
    {
    public:
        Entity CreateEntity(std::string name = "Entity");
        Entity GetEntityByUUID(UUID uuid);
        Entity GetEntityByName(const std::string& name);

        void OnUpdate(double dt);
        void Render();

        Entity operator[](const std::string& name);

        template <typename System>
        void RegisterSystem()
        {
            m_Storage.RegisterSystem<System>();
        }

    private:
        void OnEntityDestroy(Entity entity);
        FPSCamera& GetCamera();

        ECSStorage m_Storage;
        std::unordered_map<uint64_t, EntityId> m_EntityByUUID;
        std::unordered_map<std::string, EntityId> m_EntityByName;

        friend class Entity;
    };
}  // namespace DE