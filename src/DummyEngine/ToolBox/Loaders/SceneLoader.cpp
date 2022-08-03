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

    template <> YAML::Node GetComponentNode(Tag component)
    {
        YAML::Node n_Component;
        n_Component["Tag"] = component.tag;
        return n_Component;
    }
    template <> YAML::Node GetComponentNode(Id component)
    {
        YAML::Node n_Component;
        n_Component["UUID"] = (uint64_t)component;
        return n_Component;
    }
    template <> YAML::Node GetComponentNode(Transformation component)
    {
        YAML::Node n_Component;
        n_Component["Transformation"]["Translation"] = NodeVec3(component.translation);
        n_Component["Transformation"]["TranslationOffset"] = NodeVec3(component.translation_offset);
        n_Component["Transformation"]["Rotation"] = NodeVec3(component.rotation);
        n_Component["Transformation"]["RotationOffset"] = NodeVec3(component.rotation_offet);
        n_Component["Transformation"]["Scale"] = NodeVec3(component.scale);
        n_Component["Transformation"]["ScaleOffset"] = NodeVec3(component.scale_offset);
        return n_Component;
    }
    template <> YAML::Node GetComponentNode(RenderModel component)
    {
        YAML::Node n_Component;

        n_Component["RenderModel"] = fs::relative(component.path, Config::GetPath(DE_CFG_EXECUTABLE_PATH)).string();

        return n_Component;
    }

    template <typename ComponentType> void TryToSaveComponent(YAML::Node& n_Entity, Entity entity)
    {
        if (entity.HasComponent<ComponentType>())
            n_Entity.push_back(GetComponentNode(entity.GetComponent<ComponentType>()));
    }

    YAML::Node SaveEntity(Entity entity)
    {
        YAML::Node n_Entity;

        TryToSaveComponent<Tag>(n_Entity, entity);
        TryToSaveComponent<Id>(n_Entity, entity);
        TryToSaveComponent<RenderModel>(n_Entity, entity);
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