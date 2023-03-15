#include "DummyEngine/Core/Scene/Scene.h"
#include "DummyEngine/Core/ECS/Entity.hpp"
#include "DummyEngine/Core/Scene/Components.h"
#include "DummyEngine/Core/Objects/LightSources/LightSource.h"
#include "DummyEngine/Core/Rendering/Renderer/Renderer.h"
#include "DummyEngine/Core/ResourceManaging/ResourceManager.h"
#include "DummyEngine/Core/Scene/SceneRenderData.h"
#include "DummyEngine/Core/Scene/SceneHierarchy.h"
#include "DummyEngine/Core/Scripting/Script.h"
#include "DummyEngine/Core/Scripting/ScriptEngine.h"

namespace DE
{

    Scene::Scene(const std::string& name) :
        m_Name(name), m_RenderData(CreateRef<SceneRenderData>(this)), m_HierarchyRoot(CreateRef<SceneHierarchyNode>())
    {
        m_Storage.SetAddHandler<FPSCamera>([this](Entity entity) { m_RenderData->AddVPEntity(entity); });
        m_Storage.SetAddHandler<ScriptComponent>([this](Entity entity) { entity.Get<ScriptComponent>()->AttachToScene(this, entity); });
        m_Storage.SetRemoveHandler<ScriptComponent>([this](Entity entity) { entity.Get<ScriptComponent>().Destroy(); });
    }

    Entity Scene::CreateHiddenEntity(const std::string& name)
    {
        Entity      new_entity = m_Storage.CreateEntity();
        IdComponent id;

        DE_ASSERT(m_EntityByUUID.find(id) == m_EntityByUUID.end(), "UUID collision occured.");

        new_entity.AddComponent(TagComponent(name));
        new_entity.AddComponent(id);

        m_EntityByUUID[id] = new_entity;

        return new_entity;
    }
    Entity Scene::CreateEntity(const std::string& name)
    {
        Entity      new_entity = m_Storage.CreateEntity();
        IdComponent id;

        DE_ASSERT(m_EntityByUUID.find(id) == m_EntityByUUID.end(), "UUID collision occured.");
        DE_ASSERT(m_EntityByName.find(name) == m_EntityByName.end(), "Name collision occured.");

        new_entity.AddComponent(TagComponent(name));
        new_entity.AddComponent(id);

        m_EntityByUUID[id]   = new_entity;
        m_EntityByName[name] = new_entity;

        return new_entity;
    }
    Entity Scene::CreateEntityWithUUID(UUID uuid, const std::string& name)
    {
        Entity new_entity = m_Storage.CreateEntity();

        DE_ASSERT(m_EntityByUUID.find(uuid) == m_EntityByUUID.end(), "UUID collision occured.");
        DE_ASSERT(m_EntityByName.find(name) == m_EntityByName.end(), "Name collision occured.");

        new_entity.AddComponent(TagComponent(name));
        new_entity.AddComponent(IdComponent(uuid));

        m_EntityByUUID[uuid] = new_entity;
        m_EntityByName[name] = new_entity;

        return new_entity;
    }
    Entity Scene::CloneEntity(const std::string& entity_to_clone, const std::string& new_name)
    {
        DE_ASSERT(false, "Clone of entity not implemented yet.");
        // EntityId    entity_id = m_Storage.CopyEntity(m_EntityByName[entity_to_clone]);
        // Entity      new_entity(entity_id, this);
        // IdComponent id;

        // DE_ASSERT(m_EntityByName.find(new_name) == m_EntityByName.end(), "Name collision occured.");

        // new_entity.GetComponent<TagComponent>() = new_name;
        // new_entity.GetComponent<IdComponent>()  = id;

        // m_EntityByUUID[id]       = entity_id;
        // m_EntityByName[new_name] = entity_id;

        return m_Storage.CreateEntity();
    }
    Entity Scene::GetByUUID(UUID uuid)
    {
        DE_ASSERT(m_EntityByUUID.find(uuid) != m_EntityByUUID.end(), "Entity with name " + std::to_string(uuid) + " not found.");
        return m_EntityByUUID.at(uuid);
    }
    Entity Scene::GetByName(const std::string& name)
    {
        DE_ASSERT(m_EntityByName.find(name) != m_EntityByName.end(), "Entity with name " + name + " not found.");
        return m_EntityByName.at(name);
    }
    std::string             Scene::GetName() const { return m_Name; }
    Ref<SceneHierarchyNode> Scene::GetHierarchy() { return m_HierarchyRoot; }

    void Scene::OnUpdate(double dt)
    {
        DE_PROFILE_SCOPE("Scene OnUpdate");

        m_Storage.UpdateSystems(dt);
        UpdateScripts(dt);
    }

    void Scene::OnViewPortResize(uint32_t x, uint32_t y)
    {
        double aspect  = double(x) / double(y);
        auto   cameras = m_Storage.View<FPSCamera>();
        for (auto e : cameras)
        {
            e.Get<FPSCamera>().SetAspect(aspect);
        }
    }
    void Scene::Render()
    {
        m_RenderData->SetCamera(GetCamera());
        m_RenderData->Render();
    }

    Entity Scene::CreateEmptyEntity() { return m_Storage.CreateEntity(); }
    void   Scene::UpdateEmptyEntity(Entity entity)
    {
        IdComponent  id  = entity.Get<IdComponent>();
        TagComponent tag = entity.Get<TagComponent>();

        DE_ASSERT(m_EntityByUUID.find(id) == m_EntityByUUID.end(), "UUID collision occured.");
        DE_ASSERT(m_EntityByName.find(tag) == m_EntityByName.end(), "Name collision occured.");

        m_EntityByUUID[id]  = entity;
        m_EntityByName[tag] = entity;
    }
    void Scene::OnEntityDestroy(Entity entity)
    {
        m_EntityByUUID.erase(entity.Get<IdComponent>());
        m_EntityByName.erase(entity.Get<TagComponent>());
    }

    Entity Scene::GetCamera()
    {
        auto cameras = m_Storage.View<FPSCamera>();
        DE_ASSERT(!cameras.Empty(), "No available camera in scene.");
        return *cameras.begin();
    }
    void Scene::UpdateScripts(float dt)
    {
        for (auto e : m_Storage.View<ScriptComponent>())
        {
            auto& component = e.Get<ScriptComponent>();
            if (component.Valid())
            {
                component->OnUpdate(dt);
            }
        }
    }
}  // namespace DE