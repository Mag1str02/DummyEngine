#include "Core/Scene/Scene.h"
#include "Core/ECS/Entity.hpp"
#include "Core/Scene/Components.h"
#include "Core/Objects/LightSources/LightSource.h"
#include "Core/Rendering/Renderer/Renderer.h"
#include "Core/ResourceManaging/ResourceManager.h"
#include "Core/Scene/SceneRenderData.h"
#include "Core/Scripting/ScriptInstance.hpp"

namespace DE
{

    Scene::Scene(const std::string& name) : m_Name(name), m_RenderData(CreateRef<SceneRenderData>(this)) {}

    Entity Scene::CreateEntity(const std::string& name)
    {
        EntityId    entity_id = m_Storage.CreateEntity();
        Entity      new_entity(entity_id, this);
        IdComponent id;

        DE_ASSERT(m_EntityByUUID.find(id) == m_EntityByUUID.end(), "UUID collision occured.");
        DE_ASSERT(m_EntityByName.find(name) == m_EntityByName.end(), "Name collision occured.");

        new_entity.AddComponent(TagComponent(name));
        new_entity.AddComponent(id);

        m_EntityByUUID[id]   = entity_id;
        m_EntityByName[name] = entity_id;

        return new_entity;
    }
    Entity Scene::CreateEntityWithUUID(UUID uuid, const std::string& name)
    {
        EntityId entity_id = m_Storage.CreateEntity();
        Entity   new_entity(entity_id, this);

        DE_ASSERT(m_EntityByUUID.find(uuid) == m_EntityByUUID.end(), "UUID collision occured.");
        DE_ASSERT(m_EntityByName.find(name) == m_EntityByName.end(), "Name collision occured.");

        new_entity.AddComponent(TagComponent(name));
        new_entity.AddComponent(IdComponent(uuid));

        m_EntityByUUID[uuid] = entity_id;
        m_EntityByName[name] = entity_id;

        return new_entity;
    }
    Entity Scene::CloneEntity(const std::string& entity_to_clone, const std::string& new_name)
    {
        EntityId    entity_id = m_Storage.CopyEntity(m_EntityByName[entity_to_clone]);
        Entity      new_entity(entity_id, this);
        IdComponent id;

        DE_ASSERT(m_EntityByUUID.find(id) == m_EntityByUUID.end(), "UUID collision occured.");
        DE_ASSERT(m_EntityByName.find(new_name) == m_EntityByName.end(), "Name collision occured.");

        new_entity.GetComponent<TagComponent>() = new_name;
        new_entity.GetComponent<IdComponent>()  = id;

        m_EntityByUUID[id]       = entity_id;
        m_EntityByName[new_name] = entity_id;

        return new_entity;
    }
    Entity Scene::GetByUUID(UUID uuid)
    {
        DE_ASSERT(m_EntityByUUID.find(uuid) != m_EntityByUUID.end(), "Entity with name " + std::to_string(uuid) + " not found.");
        return Entity(m_EntityByUUID.at(uuid), this);
    }
    Entity Scene::GetByName(const std::string& name)
    {
        DE_ASSERT(m_EntityByName.find(name) != m_EntityByName.end(), "Entity with name " + name + " not found.");
        return Entity(m_EntityByName.at(name), this);
    }
    std::string         Scene::GetName() const { return m_Name; }
    std::vector<Entity> Scene::GetAllEntities()
    {
        std::vector<Entity> res;
        res.reserve(m_EntityByName.size());
        for (auto& [uuid, entity_id] : m_EntityByUUID)
        {
            res.push_back(Entity(entity_id, this));
        }
        std::sort(
            res.begin(), res.end(), [](Entity& a, Entity& b) { return a.GetComponent<TagComponent>().tag < b.GetComponent<TagComponent>().tag; });
        return res;
    }

    void Scene::OnUpdate(double dt)
    {
        DE_PROFILE_SCOPE("Scene OnUpdate");

        UpdateScripts(dt);
        m_Storage.UpdateSystems(dt);
    }

    void Scene::OnViewPortResize(uint32_t x, uint32_t y)
    {
        double aspect  = double(x) / double(y);
        auto&  cameras = m_Storage.GetComponentArray<FPSCamera>();
        for (auto [id, camera] : cameras)
        {
            camera.SetAspect(aspect);
        }
    }
    void Scene::UpdateScripts(double dt)
    {
        DE_PROFILE_SCOPE("Scene UpdateScripts");
        auto& scripts = m_Storage.GetComponentArray<ScriptComponent>();
        for (auto [id, script] : scripts)
        {
            script.instance->OnUpdate(dt);
        }
    }
    void Scene::Render() { m_RenderData->Render(); }

    Entity Scene::CreateEmptyEntity()
    {
        EntityId entity_id = m_Storage.CreateEntity();
        Entity   new_entity(entity_id, this);

        return new_entity;
    }
    void Scene::UpdateEmptyEntity(Entity entity)
    {
        IdComponent  id  = entity.GetComponent<IdComponent>();
        TagComponent tag = entity.GetComponent<TagComponent>();

        DE_ASSERT(m_EntityByUUID.find(id) == m_EntityByUUID.end(), "UUID collision occured.");
        DE_ASSERT(m_EntityByName.find(tag) == m_EntityByName.end(), "Name collision occured.");

        m_EntityByUUID[id]  = entity.m_Id;
        m_EntityByName[tag] = entity.m_Id;
    }
    void Scene::OnEntityDestroy(Entity entity)
    {
        m_EntityByUUID.erase(entity.GetComponent<IdComponent>());
        m_EntityByName.erase(entity.GetComponent<TagComponent>());
    }

    Entity Scene::GetCamera()
    {
        DE_ASSERT(m_Storage.GetComponentArray<FPSCamera>().begin() != m_Storage.GetComponentArray<FPSCamera>().end(),
                  "No available camera in scene.");
        return Entity((*m_Storage.GetComponentArray<FPSCamera>().begin()).first, this);
    }

    template <typename Component> void OnComponentAttach(Entity entity) { static_assert(sizeof(Component) != 0); }
    template <> void                   Scene::OnComponentAttach<FPSCamera>(Entity entity) { m_RenderData->AddVPEntity(entity); }
    template <> void                   Scene::OnComponentAttach<TagComponent>(Entity entity) {}
    template <> void                   Scene::OnComponentAttach<IdComponent>(Entity entity) {}
    template <> void                   Scene::OnComponentAttach<TransformComponent>(Entity entity) {}
    template <> void                   Scene::OnComponentAttach<ShaderComponent>(Entity entity) {}
    template <> void                   Scene::OnComponentAttach<RenderMeshComponent>(Entity entity) {}
    template <> void                   Scene::OnComponentAttach<LightSource>(Entity entity) {}
    template <> void                   Scene::OnComponentAttach<SkyBox>(Entity entity) {}
    template <> void                   Scene::OnComponentAttach<ScriptComponent>(Entity entity)
    {
        entity.GetComponent<ScriptComponent>().instance->AttachToEntity(this, entity);
    }

}  // namespace DE