#include "Core/Scene/Scene.h"
#include "Core/ECS/Entity.hpp"
#include "Core/Scene/Components.h"
#include "Core/Rendering/Renderer/Renderer.h"

namespace DE
{

    Scene::Scene(const std::string& name) : m_Name(name) {}

    Entity Scene::CreateEntity(const std::string& name)
    {
        EntityId entity_id = m_Storage.CreateEntity();
        Entity new_entity(entity_id, this);
        Id id;

        DE_ASSERT(m_EntityByUUID.find(id) == m_EntityByUUID.end(), "UUID collision occured.");
        DE_ASSERT(m_EntityByName.find(name) == m_EntityByName.end(), "Name collision occured.");

        new_entity.AddComponent(Tag(name));
        new_entity.AddComponent(id);

        m_EntityByUUID[id] = entity_id;
        m_EntityByName[name] = entity_id;

        return new_entity;
    }
    Entity Scene::CreateEntityWithUUID(UUID uuid, const std::string& name)
    {
        EntityId entity_id = m_Storage.CreateEntity();
        Entity new_entity(entity_id, this);

        DE_ASSERT(m_EntityByUUID.find(uuid) == m_EntityByUUID.end(), "UUID collision occured.");
        DE_ASSERT(m_EntityByName.find(name) == m_EntityByName.end(), "Name collision occured.");

        new_entity.AddComponent(Tag(name));
        new_entity.AddComponent(Id(uuid));

        m_EntityByUUID[uuid] = entity_id;
        m_EntityByName[name] = entity_id;

        return new_entity;
    }
    Entity Scene::CloneEntity(const std::string& entity_to_clone, const std::string& new_name)
    {
        EntityId entity_id = m_Storage.CopyEntity(m_EntityByName[entity_to_clone]);
        Entity new_entity(entity_id, this);
        Id id;

        DE_ASSERT(m_EntityByUUID.find(id) == m_EntityByUUID.end(), "UUID collision occured.");
        DE_ASSERT(m_EntityByName.find(new_name) == m_EntityByName.end(), "Name collision occured.");

        new_entity.GetComponent<Tag>() = new_name;
        new_entity.GetComponent<Id>() = id;

        m_EntityByUUID[id] = entity_id;
        m_EntityByName[new_name] = entity_id;

        return new_entity;
    }
    Entity Scene::GetByUUID(UUID uuid)
    {
        DE_ASSERT(m_EntityByUUID.find(uuid) != m_EntityByUUID.end(),
                  "Entity with name " + std::to_string(uuid) + " not found.");
        return Entity(m_EntityByUUID.at(uuid), this);
    }
    Entity Scene::GetByName(const std::string& name)
    {
        DE_ASSERT(m_EntityByName.find(name) != m_EntityByName.end(), "Entity with name " + name + " not found.");
        return Entity(m_EntityByName.at(name), this);
    }

    void Scene::OnUpdate(double dt)
    {
        m_Storage.UpdateSystems(dt);
    }
    void Scene::Render()
    {
        auto& camera = GetCamera();

        auto models = m_Storage.GetComponentArray<RenderModel>();
        auto shaders = m_Storage.GetComponentArray<Ref<Shader>>();
        auto unique_shaders = m_Storage.GetComponentArray<UniqueShader>();
        auto transformations = m_Storage.GetComponentArray<Transformation>();

        Renderer::Clear();

        for (auto [entity_id, unique_shader] : unique_shaders)
        {
            auto shader = unique_shader.shader;
            shader->Bind();
            shader->SetMat4("u_ViewProjection", camera.GetViewProjection());
            shader->SetFloat3("u_CameraPos", camera.GetPos());
        }
        for (auto [entity_id, model] : models)
        {
            DE_ASSERT(shaders[entity_id] != nullptr, "Bad Shader.");
            Renderer::Submit(shaders[entity_id], model, transformations[entity_id].GetTransform());
        }
    }

    Entity Scene::operator[](const std::string& name)
    {
        DE_ASSERT(m_EntityByName.find(name) != m_EntityByName.end(), "Entity with name " + name + " not found.");
        return Entity(m_EntityByName.at(name), this);
    }

    std::string Scene::GetName() const
    {
        return m_Name;
    }

    Entity Scene::CreateEmptyEntity()
    {
        EntityId entity_id = m_Storage.CreateEntity();
        Entity new_entity(entity_id, this);

        return new_entity;
    }
    void Scene::UpdateEmptyEntity(Entity entity)
    {
        Id id = entity.GetComponent<Id>();
        Tag tag = entity.GetComponent<Tag>();

        DE_ASSERT(m_EntityByUUID.find(id) == m_EntityByUUID.end(), "UUID collision occured.");
        DE_ASSERT(m_EntityByName.find(tag) == m_EntityByName.end(), "Name collision occured.");

        m_EntityByUUID[id] = entity.m_Id;
        m_EntityByName[tag] = entity.m_Id;
    }

    FPSCamera& Scene::GetCamera()
    {
        DE_ASSERT(m_Storage.GetComponentArray<FPSCamera>().begin() != m_Storage.GetComponentArray<FPSCamera>().end(),
                  "No available camera in scene.");
        return (*m_Storage.GetComponentArray<FPSCamera>().begin()).second;
    }
    void Scene::OnEntityDestroy(Entity entity)
    {
        m_EntityByUUID.erase(entity.GetComponent<Id>());
    }
}  // namespace DE