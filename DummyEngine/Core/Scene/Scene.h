#pragma once

#include "DummyEngine/Core/ECS/ECS.h"
#include "DummyEngine/Core/Objects/Cameras/FPSCamera.h"
#include "DummyEngine/Core/Scene/Components.h"
#include "DummyEngine/Core/Scene/SceneHierarchy.h"

namespace DE {
    class Entity;
    class SceneRenderData;
    class ScripyEngine;

    class Scene {
    public:
        Scene();
        ~Scene();

        Entity CreateEmptyEntity();
        Entity CreateEntity(const std::string& name = "Entity", bool visisble = true);
        Entity CloneEntity(Entity entity);

        bool   ExistsEntityWithID(UUID id);
        Entity GetByID(UUID uuid);

        Entity               GetCamera();
        SceneHierarchy::Node GetHierarchyRoot();
        // TODO: remove when render graph implemented
        Ref<SceneRenderData> GetRenderData() { return m_RenderData; }

        void OnUpdate(double dt);
        void OnViewPortResize(U32 x, U32 y);
        void UpdateScripts(float dt);
        void Render();

        template <typename System> void                              RegisterSystem() { m_Storage->RegisterSystem<System>(); }
        template <typename... Components> StorageView<Components...> View() { return m_Storage->View<Components...>(); }

    private:
        friend class ScriptEngine;
        friend class SceneRenderData;

        Ref<Storage>                     m_Storage;
        Ref<SceneRenderData>             m_RenderData;
        SceneHierarchy                   m_Hierarchy;
        std::unordered_map<UUID, Entity> m_EntityByID;
    };
}  // namespace DE