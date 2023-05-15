#pragma once

#include "DummyEngine/Core/ECS/ECS.h"
#include "DummyEngine/Core/Objects/Cameras/FPSCamera.h"
#include "DummyEngine/Core/Scene/Components.h"
#include "DummyEngine/Core/Scene/SceneHierarchy.h"

namespace DE {
    class Entity;
    class SceneRenderer;
    class ScripyEngine;

    class Scene {
    public:
        Scene();
        ~Scene();

        void OnRuntimeStart();
        void OnRuntimeStop();
        void OnUpdate(float dt);
        void OnRender(Entity camera = Entity());
        void OnViewPortResize(U32 x, U32 y);

        Entity CreateEmptyEntity();
        Entity CreateEntity(const std::string& name = "Entity", bool visisble = true);
        Entity CloneEntity(Entity entity);
        Entity GetByID(UUID uuid);

        Ref<SceneRenderer>   GetRenderer() { return m_Renderer; }
        SceneHierarchy::Node GetHierarchyRoot();

        template <typename System> void                              RegisterSystem() { m_Storage->RegisterSystem<System>(); }
        template <typename... Components> StorageView<Components...> View() { return m_Storage->View<Components...>(); }

    private:
        friend class ScriptEngine;
        friend class SceneRenderer;

        Ref<Storage>                     m_Storage;
        Ref<SceneRenderer>               m_Renderer;
        SceneHierarchy                   m_Hierarchy;
        std::unordered_map<UUID, Entity> m_EntityByID;
    };
}  // namespace DE