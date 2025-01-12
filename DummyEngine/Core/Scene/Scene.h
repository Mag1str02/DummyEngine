#pragma once

#include "DummyEngine/Core/ECS/ECS.h"
#include "DummyEngine/Core/Scene/SceneHierarchy.h"
#include "DummyEngine/Utils/Debug/Logger.h"

namespace DummyEngine {

    class Entity;
    class SceneRenderer;
    class ScripyEngine;

    namespace Physics {
        class Solver;
    }

    class Scene {
        LOG_AUTHOR(Scene)
    public:
        Scene();
        ~Scene();

        void OnRuntimeStart();
        void OnRuntimePause();
        void OnRuntimeResume();
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

        void LoadPhysics(Ref<Scene>& scene);

        Ref<SceneRenderer>   GetRenderer() { return renderer_; }
        SceneHierarchy::Node GetHierarchyRoot();

        template <typename System> Ref<System> AttachSystem(Ref<System> system = nullptr) {
            if (!system) {
                system = CreateRef<System>();
            }
            storage_->AttachSystem<System>(system);
            return system;
        }
        template <typename... Components> StorageView<Components...> View() { return storage_->View<Components...>(); }

    private:
        friend class ScriptEngine;
        friend class SceneRenderer;

        Entity                           camera_;
        Ref<Storage>                     storage_;
        Ref<SceneRenderer>               renderer_;
        Ref<Physics::Solver>             physics_solver_;
        SceneHierarchy                   hierarchy_;
        std::unordered_map<UUID, Entity> entity_by_id_;
    };

}  // namespace DummyEngine