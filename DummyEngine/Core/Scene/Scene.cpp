#include "DummyEngine/Core/Scene/Scene.h"

#include "DummyEngine/Core/ECS/Entity.hpp"
#include "DummyEngine/Core/Objects/LightSources/LightSource.h"
#include "DummyEngine/Core/Rendering/Renderer/Renderer.h"
#include "DummyEngine/Core/ResourceManaging/ResourceManager.h"
#include "DummyEngine/Core/Scene/Components.h"
#include "DummyEngine/Core/Scene/SceneHierarchy.h"
#include "DummyEngine/Core/Scene/SceneRenderData.h"
#include "DummyEngine/Core/Scripting/Script.h"
#include "DummyEngine/Core/Scripting/ScriptEngine.h"

namespace DE {
    Scene::Scene() : m_Storage(CreateRef<Storage>()), m_RenderData(CreateRef<SceneRenderData>(this)), m_Hierarchy("Scene") {
        m_Storage->SetAddHandler<FPSCamera>([this](Entity entity) { m_RenderData->AddVPEntity(entity); });
        m_Storage->SetAddHandler<TagComponent>([this](Entity entity) {
            auto& name = entity.Get<TagComponent>();
            DE_ASSERT(m_EntityByTag.find(name) == m_EntityByTag.end(), "Name collision occured (", name.Get(), ")");
            m_EntityByTag[name] = entity;
        });
        m_Storage->SetAddHandler<IDComponent>([this](Entity entity) {
            auto id = entity.Get<IDComponent>();
            DE_ASSERT(m_EntityByID.find(id) == m_EntityByID.end(), "UUID collision occured (", id.Get(), ")");
            m_EntityByID[id] = entity;
        });
        m_Storage->SetRemoveHandler<TagComponent>([this](Entity entity) { m_EntityByTag.erase(entity.Get<TagComponent>()); });
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
        new_entity.AddComponent(TagComponent(GenAvilableEntityName(name)));
        new_entity.AddComponent(IDComponent(UUID::Generate()));
        if (visisble) {
            m_Hierarchy.AddEntity(new_entity);
        }
        return new_entity;
    }
    Entity Scene::CloneEntity(const std::string& entity_to_clone, const std::string& new_name) {
        DE_ASSERT(false, "Clone of entity not implemented yet.");
        return m_Storage->CreateEntity();
    }
    bool Scene::ExistsEntityWithTag(const TagComponent& name) {
        return m_EntityByTag.contains(name);
    }
    bool Scene::ExistsEntityWithID(UUID id) {
        return m_EntityByID.contains(id);
    }

    Entity Scene::GetByID(UUID uuid) {
        return (m_EntityByID.contains(uuid) ? m_EntityByID.at(uuid) : Entity());
    }
    Entity Scene::GetByTag(const std::string& name) {
        return (m_EntityByTag.contains(name) ? m_EntityByTag.at(name) : Entity());
    }

    SceneHierarchy& Scene::GetHierarchy() {
        return m_Hierarchy;
    }

    void Scene::OnUpdate(double dt) {
        DE_PROFILE_SCOPE("Scene OnUpdate");

        m_Storage->UpdateSystems(dt);
        UpdateScripts(dt);
    }

    void Scene::OnViewPortResize(U32 x, U32 y) {
        double aspect  = double(x) / double(y);
        auto   cameras = m_Storage->View<FPSCamera>();
        for (auto e : cameras) {
            e.Get<FPSCamera>().SetAspect(aspect);
        }
    }
    void Scene::Render() {
        m_RenderData->SetCamera(GetCamera());
        m_RenderData->Render();
    }

    Entity Scene::GetCamera() {
        auto cameras = m_Storage->View<FPSCamera>();
        DE_ASSERT(!cameras.Empty(), "No available camera in scene.");
        return *cameras.begin();
    }
    void Scene::UpdateScripts(float dt) {
        for (auto e : m_Storage->View<ScriptComponent>()) {
            auto& component = e.Get<ScriptComponent>();
            if (component.Valid()) {
                component->OnUpdate(dt);
            }
        }
    }

    std::string Scene::GenAvilableEntityName(const std::string& prefered) {
        std::string name = prefered;
        U32         cnt  = 0;
        while (ExistsEntityWithTag(name)) {
            name = StrCat(prefered, "(", ++cnt, ")");
        }
        return name;
    }
}  // namespace DE