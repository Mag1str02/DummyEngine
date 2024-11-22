#include "Scene.h"

#include "DummyEngine/Core/Physics/Solver.hpp"
#include "DummyEngine/Core/Scene/Components.h"
#include "DummyEngine/Core/Scene/SceneHierarchy.h"
#include "DummyEngine/Core/Scene/SceneRenderer.h"
#include "DummyEngine/Core/Scripting/Script.h"
#include "DummyEngine/Core/Scripting/ScriptEngine.h"
#include "DummyEngine/Utils/Debug/Profiler.h"

#include <unordered_set>

namespace DummyEngine {
    void Scene::OnRuntimeStart() {
        for (auto e : storage_->View<ScriptComponent>()) {
            auto& component = e.Get<ScriptComponent>();
            if (component.Valid()) {
                component->OnRuntimeStart();
            }
        }
        std::unordered_set<Ref<RenderMesh>> meshes;
        for (auto e : storage_->View<RenderMeshComponent>()) {
            auto mesh = e.Get<RenderMeshComponent>().Mesh;
            if (mesh->Animator && !meshes.contains(mesh)) {
                meshes.insert(mesh);
                mesh->Animator->SetTime(0);
            }
        }
    }

    void Scene::OnRuntimePause() {}

    void Scene::OnRuntimeResume() {}

    void Scene::OnRuntimeStop() {
        for (auto e : storage_->View<ScriptComponent>()) {
            auto& component = e.Get<ScriptComponent>();
            if (component.Valid()) {
                component->OnRuntimeStop();
            }
        }
        std::unordered_set<Ref<RenderMesh>> meshes;
        for (auto e : storage_->View<RenderMeshComponent>()) {
            auto mesh = e.Get<RenderMeshComponent>().Mesh;
            if (mesh->Animator && !meshes.contains(mesh)) {
                meshes.insert(mesh);
                mesh->Animator->SetTime(0);
            }
        }
    }
    void Scene::OnUpdate(float dt) {
        DE_PROFILE_SCOPE("Scene OnUpdate");
        if (physics_solver_) {
            DE_PROFILE_SCOPE("Physics");
            physics_solver_->OnUpdate(dt * 2);
        }
        for (auto e : storage_->View<ScriptComponent>()) {
            auto& component = e.Get<ScriptComponent>();
            if (component.Valid()) {
                component->OnUpdate(dt);
            }
        }
        std::unordered_set<Ref<RenderMesh>> meshes;
        for (auto e : storage_->View<RenderMeshComponent>()) {
            auto mesh = e.Get<RenderMeshComponent>().Mesh;
            if (mesh->Animator && !meshes.contains(mesh)) {
                meshes.insert(mesh);
                mesh->Animator->UpdateAnimation(dt);
            }
        }
        storage_->UpdateSystems(dt);
    }
    void Scene::OnRender(Entity camera) {
        if (!camera.Valid()) {
            camera = camera_;
        }
        if (!camera.Has<FPSCamera>()) {
            LOG_WARNING("Specified camera entity has no camera component");
            return;
        }
        renderer_->Render(camera);
    }

    void Scene::OnViewPortResize(U32 x, U32 y) {
        double aspect  = double(x) / double(y);
        auto   cameras = storage_->View<FPSCamera>();
        for (auto e : cameras) {
            e.Get<FPSCamera>().SetAspect(aspect);
        }
        renderer_->OnViewPortResize(x, y);
    }

    Scene::Scene() : storage_(CreateRef<Storage>()), renderer_(CreateRef<SceneRenderer>(this)), hierarchy_("Scene") {
        storage_->SetAddHandler<IDComponent>([this](Entity entity) {
            auto id = entity.Get<IDComponent>();
            DE_ASSERT(entity_by_id_.find(id) == entity_by_id_.end(), "UUID collision occured {}", id.Get());
            entity_by_id_[id] = entity;
        });
        physics_solver_ = CreateRef<Physics::Solver>();
        storage_->SetRemoveHandler<IDComponent>([this](Entity entity) { entity_by_id_.erase(entity.Get<IDComponent>()); });
    }

    Scene::~Scene() {
        storage_->Destruct();
        storage_ = nullptr;
    }

    Entity Scene::CreateEmptyEntity() {
        return storage_->CreateEntity();
    }
    Entity Scene::CreateEntity(const std::string& name, bool visisble) {
        Entity new_entity = storage_->CreateEntity();
        new_entity.AddComponent(TagComponent(name));
        new_entity.AddComponent(IDComponent(UUID::Generate()));
        if (visisble) {
            hierarchy_.AddEntity(new_entity);
        }
        return new_entity;
    }
    Entity Scene::CloneEntity(Entity) {
        return CreateEntity("Entity", false);
    }

    Entity Scene::GetByID(UUID uuid) {
        return (entity_by_id_.contains(uuid) ? entity_by_id_.at(uuid) : Entity());
    }
    SceneHierarchy::Node Scene::GetHierarchyRoot() {
        return hierarchy_.GetRoot();
    }
    void Scene::SetCamera(Entity entity) {
        DE_ASSERT(entity.Has<FPSCamera>(), "SetCamera on entity withour camera");
        camera_ = entity;
    }
    bool Scene::HasCamera() {
        return camera_.Valid();
    }
    void Scene::LoadPhysics(Ref<Scene>& scene) {
        physics_solver_->LoadScene(scene);
    }
}  // namespace DummyEngine