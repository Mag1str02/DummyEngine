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
        for (auto e : m_Storage->View<ScriptComponent>()) {
            auto& component = e.Get<ScriptComponent>();
            if (component.Valid()) {
                component->OnRuntimeStart();
            }
        }
    }
    void Scene::OnRuntimeStop() {
        for (auto e : m_Storage->View<ScriptComponent>()) {
            auto& component = e.Get<ScriptComponent>();
            if (component.Valid()) {
                component->OnRuntimeStop();
            }
        }
    }
    void Scene::OnUpdate(float dt) {
        DE_PROFILE_SCOPE("Scene OnUpdate");

        m_Storage->UpdateSystems(dt);
        for (auto e : m_Storage->View<ScriptComponent>()) {
            auto& component = e.Get<ScriptComponent>();
            if (component.Valid()) {
                component->OnUpdate(dt);
            }
        }
    }
    void Scene::OnRender(Entity camera) {
        if (!camera.Valid()) {
            auto cameras = View<FPSCamera>();
            if (cameras.Empty()) {
                LOG_WARNING("No camera available in scene");
                return;
            }
            camera = *cameras.begin();
        }
        if (!camera.Has<FPSCamera>()) {
            LOG_WARNING("Specified camera entity has no camera component");
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
        m_Storage->SetAddHandler<FPSCamera>([this](Entity entity) { m_Renderer->AddVPEntity(entity); });
        m_Storage->SetAddHandler<IDComponent>([this](Entity entity) {
            auto id = entity.Get<IDComponent>();
            DE_ASSERT(m_EntityByID.find(id) == m_EntityByID.end(), "UUID collision occured (", id.Get(), ")");
            m_EntityByID[id] = entity;
        });
        m_Storage->SetRemoveHandler<IDComponent>([this](Entity entity) { m_EntityByID.erase(entity.Get<IDComponent>()); });
        m_Storage->SetRemoveHandler<ScriptComponent>([this](Entity entity) { entity.Get<ScriptComponent>().Destroy(); });
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
        DE_ASSERT(false, "Clone of entity not implemented yet.");
        return CreateEntity("Entity", false);
    }

    Entity Scene::GetByID(UUID uuid) {
        return (m_EntityByID.contains(uuid) ? m_EntityByID.at(uuid) : Entity());
    }
    SceneHierarchy::Node Scene::GetHierarchyRoot() {
        return m_Hierarchy.GetRoot();
    }
}  // namespace DE