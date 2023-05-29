#pragma once

#include "DummyEngine/Core/ECS/ECS.h"
#include "DummyEngine/Core/Objects/Cameras/FPSCamera.h"
#include "DummyEngine/Core/Physics/Solver.hpp"
#include "DummyEngine/Core/Scene/Components.h"
#include "DummyEngine/Core/Scene/SceneHierarchy.h"

namespace DE {
    class Entity;
    class SceneRenderer;
    class ScripyEngine;
    namespace Physics {
        class Solver;
    }

    class Scene{
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
        void   SetCamera(Entity entity);
        bool   HasCamera();

        void LoadPhysics(Ref<Scene> &scene);

        Ref<SceneRenderer>   GetRenderer() { return m_Renderer; }
        SceneHierarchy::Node GetHierarchyRoot();

        template <typename System> Ref<System> AttachSystem(Ref<System> system = nullptr) {
            if (!system) {
                system = CreateRef<System>();
            }
            m_Storage->AttachSystem<System>(system);
            return system;
        }
        template <typename... Components> StorageView<Components...> View() { return m_Storage->View<Components...>(); }

    private:
        friend class ScriptEngine;
        friend class SceneRenderer;

        Entity                           m_Camera;
        Ref<Storage>                     m_Storage;
        Ref<SceneRenderer>               m_Renderer;
        SceneHierarchy                   m_Hierarchy;
        std::unordered_map<UUID, Entity> m_EntityByID;
        Ref<Physics::Solver>             m_PhysicsSolver;
    };
}  // namespace DE