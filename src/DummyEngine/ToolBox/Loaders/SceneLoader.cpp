#include "Addition/Base.h"
#include "Core/ECS/Entity.hpp"
#include "ToolBox/Loaders/SceneLoader.h"

namespace DE
{
    YAML::Node NodeVec2(Vec2 vec)
    {
        YAML::Node res = YAML::Load("[]");

        res.push_back(vec.x);
        res.push_back(vec.y);

        return res;
    }
    YAML::Node NodeVec3(Vec3 vec)
    {
        YAML::Node res = YAML::Load("[]");

        res.push_back(vec.x);
        res.push_back(vec.y);
        res.push_back(vec.z);

        return res;
    }
    YAML::Node NodeVec4(Vec4 vec)
    {
        YAML::Node res = YAML::Load("[]");

        res.push_back(vec.x);
        res.push_back(vec.y);
        res.push_back(vec.z);
        res.push_back(vec.w);

        return res;
    }

    template <typename ComponentType>
    void TryToSaveComponent(YAML::Node& n_Entity, Entity entity)
    {
        if (entity.HasComponent<ComponentType>())
            n_Entity.push_back(GetComponentNode(entity.GetComponent<ComponentType>()));
    }

    YAML::Node SaveEntity(Entity entity)
    {
        YAML::Node n_Entity;

        TryToSaveComponent<Tag>(n_Entity, entity);
        TryToSaveComponent<Id>(n_Entity, entity);
        TryToSaveComponent<Transformation>(n_Entity, entity);
        return n_Entity;
    }
    void SceneLoader::SaveScene(Ref<Scene> scene, Path path)
    {
        YAML::Node n_Root, n_Scene, n_Entities;
        std::ofstream output_file;

        output_file.open(path);
        DE_ASSERT(output_file.is_open(), "Failed to open file to save scene.");

        n_Root["Scene"] = n_Scene;
        n_Scene["Name"] = scene->GetName();
        n_Scene["Entities"] = n_Entities;

        for (auto [uuid, entity_id] : scene->m_EntityByUUID)
        {
            n_Entities.push_back(SaveEntity(scene->GetEntityByUUID(UUID(uuid))));
        }

        output_file << n_Root;
    }

}  // namespace DE