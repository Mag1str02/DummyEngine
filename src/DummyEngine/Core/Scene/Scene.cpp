#include "Core/Scene/Scene.h"
#include "Core/ECS/Entity.hpp"
#include "Core/Scene/Components.h"
#include "Core/Rendering/Renderer/Renderer.h"
#include "Core/ResourceManaging/ResourceManager.h"
#include "Core/Objects/LightSources/LightSource.h"

namespace DE
{

    Scene::Scene(const std::string& name) : m_Name(name) {}

    Entity Scene::CreateEntity(const std::string& name)
    {
        EntityId entity_id = m_Storage.CreateEntity();
        Entity new_entity(entity_id, this);
        IdComponent id;

        DE_ASSERT(m_EntityByUUID.find(id) == m_EntityByUUID.end(), "UUID collision occured.");
        DE_ASSERT(m_EntityByName.find(name) == m_EntityByName.end(), "Name collision occured.");

        new_entity.AddComponent(TagComponent(name));
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

        new_entity.AddComponent(TagComponent(name));
        new_entity.AddComponent(IdComponent(uuid));

        m_EntityByUUID[uuid] = entity_id;
        m_EntityByName[name] = entity_id;

        return new_entity;
    }
    Entity Scene::CloneEntity(const std::string& entity_to_clone, const std::string& new_name)
    {
        EntityId entity_id = m_Storage.CopyEntity(m_EntityByName[entity_to_clone]);
        Entity new_entity(entity_id, this);
        IdComponent id;

        DE_ASSERT(m_EntityByUUID.find(id) == m_EntityByUUID.end(), "UUID collision occured.");
        DE_ASSERT(m_EntityByName.find(new_name) == m_EntityByName.end(), "Name collision occured.");

        new_entity.GetComponent<TagComponent>() = new_name;
        new_entity.GetComponent<IdComponent>() = id;

        m_EntityByUUID[id] = entity_id;
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
    std::string Scene::GetName() const
    {
        return m_Name;
    }

    void Scene::OnUpdate(double dt)
    {
        m_Storage.UpdateSystems(dt);
    }
    void Scene::Render()
    {
        auto& camera = GetCamera();

        auto models = m_Storage.GetComponentArray<RenderMeshComponent>();
        auto shaders = m_Storage.GetComponentArray<ShaderComponent>();
        auto transformations = m_Storage.GetComponentArray<TransformComponent>();

        Renderer::Clear();

        for (auto [id, shader] : m_RenderData.m_Shaders)
        {
            shader->Bind();
            shader->SetMat4("u_ViewProjection", camera.GetViewProjection());
            shader->SetFloat3("u_CameraPos", camera.GetPos());
        }

        LightPass();

        for (auto [entity_id, model] : models)
        {
            Renderer::Submit(shaders[entity_id].shader, model.mesh, transformations[entity_id].GetTransform());
        }
    }

    Entity Scene::operator[](const std::string& name)
    {
        DE_ASSERT(m_EntityByName.find(name) != m_EntityByName.end(), "Entity with name " + name + " not found.");
        return Entity(m_EntityByName.at(name), this);
    }

    Entity Scene::CreateEmptyEntity()
    {
        EntityId entity_id = m_Storage.CreateEntity();
        Entity new_entity(entity_id, this);

        return new_entity;
    }
    void Scene::UpdateEmptyEntity(Entity entity)
    {
        IdComponent id = entity.GetComponent<IdComponent>();
        TagComponent tag = entity.GetComponent<TagComponent>();

        DE_ASSERT(m_EntityByUUID.find(id) == m_EntityByUUID.end(), "UUID collision occured.");
        DE_ASSERT(m_EntityByName.find(tag) == m_EntityByName.end(), "Name collision occured.");

        m_EntityByUUID[id] = entity.m_Id;
        m_EntityByName[tag] = entity.m_Id;
    }
    void Scene::OnEntityDestroy(Entity entity)
    {
        m_EntityByUUID.erase(entity.GetComponent<IdComponent>());
        m_EntityByName.erase(entity.GetComponent<TagComponent>());
    }

    void Scene::LightPass()
    {
        auto& u_LightSources = m_Storage.GetComponentArray<LightSource>();
        for (auto [id, shader] : m_RenderData.m_Shaders)
        {
            shader->Bind();
            int cnt_light_sources = 0;
            for (auto [entity_id, light_source] : u_LightSources)
            {
                shader->SetFloat3("u_LightSources[" + std::to_string(cnt_light_sources) + "].m_Ambient", light_source.ambient);
                shader->SetFloat3("u_LightSources[" + std::to_string(cnt_light_sources) + "].m_Diffuse", light_source.diffuse);
                shader->SetFloat3("u_LightSources[" + std::to_string(cnt_light_sources) + "].m_Specular", light_source.specular);
                shader->SetFloat3("u_LightSources[" + std::to_string(cnt_light_sources) + "].m_Position", light_source.position);
                shader->SetFloat3("u_LightSources[" + std::to_string(cnt_light_sources) + "].m_CLQ", light_source.clq);
                shader->SetFloat3("u_LightSources[" + std::to_string(cnt_light_sources) + "].m_Direction", light_source.direction);
                shader->SetFloat3("u_LightSources[" + std::to_string(cnt_light_sources) + "].m_ConesAndType",
                                  light_source.outer_cone_cos,
                                  light_source.inner_cone_cos,
                                  LightSourceTypeToId(light_source.type));
                cnt_light_sources++;
            }
            shader->SetInt("u_LightAmount", cnt_light_sources);
        }
    }
    FPSCamera& Scene::GetCamera()
    {
        DE_ASSERT(m_Storage.GetComponentArray<FPSCamera>().begin() != m_Storage.GetComponentArray<FPSCamera>().end(),
                  "No available camera in scene.");
        return (*m_Storage.GetComponentArray<FPSCamera>().begin()).second;
    }

    void Scene::RequestShader(UUID id)
    {
        if (m_RenderData.m_Shaders.find(id) == m_RenderData.m_Shaders.end())
        {
            m_RenderData.m_Shaders[id] = ResourceManager::GetResource<Shader>(id);
        }
    }
    void Scene::RequestRenderMesh(UUID id)
    {
        if (m_RenderData.m_RenderMeshes.find(id) == m_RenderData.m_RenderMeshes.end())
        {
            m_RenderData.m_RenderMeshes[id] = ResourceManager::GetResource<RenderMesh>(id);
        }
    }
}  // namespace DE