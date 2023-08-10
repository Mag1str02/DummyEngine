#include "DummyEngine/Core/Scene/Scene.h"

#include "DummyEngine/Core/ECS/Entity.hpp"
#include "DummyEngine/Core/Objects/LightSources/LightSource.h"
#include "DummyEngine/Core/Rendering/Renderer/Renderer.h"
#include "DummyEngine/Core/ResourceManaging/ResourceManager.h"
#include "DummyEngine/Core/Scene/Components.h"
#include "DummyEngine/Core/Scene/SceneHierarchy.h"
#include "DummyEngine/Core/Scene/SceneRenderer.h"
#include "DummyEngine/Core/Scripting/Script.h"
#include "DummyEngine/Core/Scripting/ScriptEngine.h"

namespace DE {
    void Scene::OnRuntimeStart() {
        for (auto e : View<AudioComponent>()) {
            auto& audio = e.Get<AudioComponent>();
            if (audio.sound) {
                audio.sound->start_streaming();
            }
        }
        for (auto e : m_Storage->View<ScriptComponent>()) {
            auto& component = e.Get<ScriptComponent>();
            if (component.Valid()) {
                component->OnRuntimeStart();
            }
        }
        std::unordered_set<Ref<RenderMesh>> meshes;
        for (auto e : m_Storage->View<RenderMeshComponent>()) {
            auto mesh = e.Get<RenderMeshComponent>().mesh;
            if (mesh->p_Animator && !meshes.contains(mesh)) {
                meshes.insert(mesh);
                mesh->p_Animator->SetTime(0);
            }
        }
    }

    void Scene::OnRuntimePause() {
        for (auto e : View<AudioComponent>()) {
            auto& audio = e.Get<AudioComponent>();
            if (audio.sound) {
                audio.sound->pause_streaming();
            }
        }
    }

    void Scene::OnRuntimeResume() {
        for (auto e : View<AudioComponent>()) {
            auto& audio = e.Get<AudioComponent>();
            if (audio.sound) {
                audio.sound->resume_streaming();
            }
        }
    }

    void Scene::OnRuntimeStop() {
        for (auto e : View<AudioComponent>()) {
            auto& audio = e.Get<AudioComponent>();
            if (audio.sound) {
                audio.sound->stop_streaming();
            }
        }
        for (auto e : m_Storage->View<ScriptComponent>()) {
            auto& component = e.Get<ScriptComponent>();
            if (component.Valid()) {
                component->OnRuntimeStop();
            }
        }
        std::unordered_set<Ref<RenderMesh>> meshes;
        for (auto e : m_Storage->View<RenderMeshComponent>()) {
            auto mesh = e.Get<RenderMeshComponent>().mesh;
            if (mesh->p_Animator && !meshes.contains(mesh)) {
                meshes.insert(mesh);
                mesh->p_Animator->SetTime(0);
            }
        }
    }
    void Scene::OnUpdate(float dt) {
        DE_PROFILE_SCOPE("Scene OnUpdate");
        if (m_PhysicsSolver) {
            DE_PROFILE_SCOPE("Physics");
            m_PhysicsSolver->OnUpdate(dt * 2);
        }
        for (auto e : m_Storage->View<ScriptComponent>()) {
            auto& component = e.Get<ScriptComponent>();
            if (component.Valid()) {
                component->OnUpdate(dt);
            }
        }
        for (auto e : m_Storage->View<AudioComponent>()) {
            auto& component = e.Get<AudioComponent>();
            if (component.sound) {
                Vec3 pos(0.0f);
                if (e.Has<TransformComponent>()) {
                    pos = e.Get<TransformComponent>().translation;
                }
                component.sound->setPosition({pos.x, pos.y, pos.z});
                component.sound->play_streaming();
            }
        }
        std::unordered_set<Ref<RenderMesh>> meshes;
        for (auto e : m_Storage->View<RenderMeshComponent>()) {
            auto mesh = e.Get<RenderMeshComponent>().mesh;
            if (mesh->p_Animator && !meshes.contains(mesh)) {
                meshes.insert(mesh);
                mesh->p_Animator->UpdateAnimation(dt);
            }
        }
        m_Storage->UpdateSystems(dt);
    }
    void Scene::OnRender(Entity camera) {
        if (!camera.Valid()) {
            camera = m_Camera;
        }
        if (!camera.Has<FPSCamera>()) {
            LOG_WARNING("Scene", "Specified camera entity has no camera component");
            return;
        }
        m_Renderer->Render(camera);
    }

    void Scene::OnViewPortResize(U32 x, U32 y) {
        double aspect  = double(x) / double(y);
        auto   cameras = m_Storage->View<FPSCamera>();
        for (auto e : cameras) {
            e.Get<FPSCamera>().SetAspect(aspect);
        }
        m_Renderer->OnViewPortResize(x, y);
    }

    Scene::Scene() : m_Storage(CreateRef<Storage>()), m_Renderer(CreateRef<SceneRenderer>(this)), m_Hierarchy("Scene") {
        m_Storage->SetAddHandler<IDComponent>([this](Entity entity) {
            auto id = entity.Get<IDComponent>();
            DE_ASSERT(m_EntityByID.find(id) == m_EntityByID.end(), "UUID collision occured (", id.Get(), ")");
            m_EntityByID[id] = entity;
        });
        m_PhysicsSolver = CreateRef<Physics::Solver>();
        m_Storage->SetRemoveHandler<IDComponent>([this](Entity entity) { m_EntityByID.erase(entity.Get<IDComponent>()); });
        m_Storage->SetRemoveHandler<AudioComponent>([this](Entity entity) {
            auto& sound = entity.Get<AudioComponent>().sound;
            if (sound) {
                sound->stop_streaming();
            }
        });
    }

    Scene::~Scene() {
        m_Storage->Destruct();
        m_Storage = nullptr;
    }

    Entity Scene::CreateEmptyEntity() {
        return m_Storage->CreateEntity();
    }
    Entity Scene::CreateEntity(const std::string& name, bool visisble) {
        Entity new_entity = m_Storage->CreateEntity();
        new_entity.AddComponent(TagComponent(name));
        new_entity.AddComponent(IDComponent(UUID::Generate()));
        if (visisble) {
            m_Hierarchy.AddEntity(new_entity);
        }
        return new_entity;
    }
    Entity Scene::CloneEntity(Entity entity) {
        // DE_ASSERT(false, "Clone of entity not implemented yet.");
        return CreateEntity("Entity", false);
    }

    Entity Scene::GetByID(UUID uuid) {
        return (m_EntityByID.contains(uuid) ? m_EntityByID.at(uuid) : Entity());
    }
    SceneHierarchy::Node Scene::GetHierarchyRoot() {
        return m_Hierarchy.GetRoot();
    }
    void Scene::SetCamera(Entity entity) {
        DE_ASSERT(entity.Has<FPSCamera>(), "SetCamera on entity withour camera");
        m_Camera = entity;
    }
    bool Scene::HasCamera() {
        return m_Camera.Valid();
    }
    void Scene::LoadPhysics(Ref<Scene>& scene) {
        m_PhysicsSolver->LoadScene(scene);
    }
}  // namespace DE