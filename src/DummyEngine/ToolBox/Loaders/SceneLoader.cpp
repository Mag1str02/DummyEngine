#include "Addition/Base.h"
#include "Core/Objects/LightSources/LightSource.h"
#include "Core/ECS/Entity.hpp"
#include "ToolBox/Loaders/SceneLoader.h"

namespace DE
{
    SceneLoader::LoaderState SceneLoader::m_State;

    std::string Relative(const Path& path)
    {
        return fs::relative(path, Config::GetPath(DE_CFG_EXECUTABLE_PATH)).string();
    }

    void SceneLoader::LoaderState::Clear()
    {
        m_ModelsPath.clear();
        m_Shaders.clear();
    }

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

    template <> YAML::Node SceneLoader::GetComponentNode(Tag component)
    {
        YAML::Node n_Component;
        n_Component["Tag"] = component.tag;
        return n_Component;
    }
    template <> YAML::Node SceneLoader::GetComponentNode(Id component)
    {
        // TODO: Save UUID in hex format.
        YAML::Node n_Component;
        n_Component["UUID"] = (uint64_t)component;
        return n_Component;
    }
    template <> YAML::Node SceneLoader::GetComponentNode(Transformation component)
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
    template <> YAML::Node SceneLoader::GetComponentNode(RenderModel component)
    {
        YAML::Node n_Component;

        m_State.m_ModelsPath.insert(Relative(component.path));
        n_Component["RenderModel"] = Relative(component.path);

        return n_Component;
    }
    template <> YAML::Node SceneLoader::GetComponentNode(Ref<Shader> component)
    {
        YAML::Node n_Component;

        m_State.m_Shaders.insert(component);
        n_Component["Shader"] = component->GetName();

        return n_Component;
    }
    template <> YAML::Node SceneLoader::GetComponentNode(UniqueShader component)
    {
        YAML::Node n_Component;

        m_State.m_Shaders.insert(component);
        n_Component["UniqueShader"] = component.shader->GetName();

        return n_Component;
    }
    template <> YAML::Node SceneLoader::GetComponentNode(FPSCamera component)
    {
        YAML::Node n_Component;

        n_Component["FPSCamera"]["FOV"] = component.m_FOV;
        n_Component["FPSCamera"]["Aspect"] = component.m_Aspect;
        n_Component["FPSCamera"]["NearPlane"] = component.m_NearPlane;
        n_Component["FPSCamera"]["FarPlane"] = component.m_FarPlane;
        n_Component["FPSCamera"]["Position"] = NodeVec3(component.m_Position);
        n_Component["FPSCamera"]["Direction"] = NodeVec3(component.m_Direction);

        return n_Component;
    }
    template <> YAML::Node SceneLoader::GetComponentNode(LightSource component)
    {
        YAML::Node n_Component;

        n_Component["LightSource"]["Type"] = LightSourceTypeToString(component.type);
        n_Component["LightSource"]["Ambient"] = NodeVec3(component.ambient);
        n_Component["LightSource"]["Diffuse"] = NodeVec3(component.diffuse);
        n_Component["LightSource"]["Specular"] = NodeVec3(component.specular);
        n_Component["LightSource"]["Direction"] = NodeVec3(component.direction);
        n_Component["LightSource"]["Position"] = NodeVec3(component.position);
        n_Component["LightSource"]["CLQ"] = NodeVec3(component.clq);
        n_Component["LightSource"]["InnerCone"] = component.inner_cone_cos;
        n_Component["LightSource"]["OuterCone"] = component.outer_cone_cos;

        return n_Component;
    }

    template <typename ComponentType> void SceneLoader::TryToSaveComponent(YAML::Node& n_Entity, Entity entity)
    {
        if (entity.HasComponent<ComponentType>())
            n_Entity.push_back(GetComponentNode(entity.GetComponent<ComponentType>()));
    }

    YAML::Node SceneLoader::SaveEntity(Entity entity)
    {
        YAML::Node n_Entity;

        TryToSaveComponent<Tag>(n_Entity, entity);
        TryToSaveComponent<Id>(n_Entity, entity);
        TryToSaveComponent<RenderModel>(n_Entity, entity);
        TryToSaveComponent<Transformation>(n_Entity, entity);
        TryToSaveComponent<Ref<Shader>>(n_Entity, entity);
        TryToSaveComponent<UniqueShader>(n_Entity, entity);
        TryToSaveComponent<FPSCamera>(n_Entity, entity);
        TryToSaveComponent<LightSource>(n_Entity, entity);

        return n_Entity;
    }
    YAML::Node SceneLoader::SaveModels()
    {
        YAML::Node n_Models;

        for (const auto& model_path : m_State.m_ModelsPath)
        {
            n_Models.push_back(model_path.string());
        }
        return n_Models;
    }
    YAML::Node SceneLoader::SaveShaders()
    {
        YAML::Node n_Shaders;

        for (const auto& shader : m_State.m_Shaders)
        {
            for (const auto& part : shader->GetParts())
            {
                n_Shaders[shader->GetName()][ShaderPartTypeToString(part.type)] = Relative(part.path);
            }
        }
        return n_Shaders;
    }

    void SceneLoader::SaveScene(Ref<Scene> scene, Path path)
    {
        YAML::Node n_Root, n_Scene, n_Entities, n_Assets;
        std::ofstream output_file;

        m_State.Clear();
        output_file.open(path);
        // TODO: Switch to throw.
        DE_ASSERT(output_file.is_open(), "Failed to open file to save scene.");

        n_Root["Scene"] = n_Scene;
        n_Scene["Name"] = scene->GetName();
        n_Scene["Assets"] = n_Assets;
        n_Scene["Entities"] = n_Entities;

        std::vector<std::pair<Tag, Entity>> entities;
        for (auto [uuid, entity_id] : scene->m_EntityByUUID)
        {
            auto entity = scene->GetEntityByUUID(UUID(uuid));
            entities.push_back({entity.GetComponent<Tag>(), entity});
        }
        std::sort(
            entities.begin(), entities.end(), [](const auto& a, const auto& b) { return a.first.tag < b.first.tag; });

        for (auto [tag, enitity] : entities)
        {
            n_Entities.push_back(SaveEntity(enitity));
        }

        n_Assets["Models"] = SaveModels();
        n_Assets["Shaders"] = SaveShaders();

        output_file << n_Root;
    }

}  // namespace DE