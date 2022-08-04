#include "Addition/Base.h"
#include "Core/Objects/LightSources/LightSource.h"
#include "Core/ECS/Entity.hpp"
#include "ToolBox/Loaders/SceneLoader.h"
#include "ToolBox/Loaders/ModelLoader.h"

namespace DE
{
    SceneLoader::LoaderState SceneLoader::m_State;

    //*~~~Additional~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    std::string Relative(const Path& path)
    {
        return fs::relative(path, Config::GetPath(DE_CFG_EXECUTABLE_PATH)).string();
    }
    void SceneLoader::LoaderState::Clear()
    {
        m_ModelsPath.clear();
        m_Shaders.clear();
    }

    //*~~~Saving~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

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

    template <typename ComponentType> void SceneLoader::TryToSaveComponent(YAML::Node& n_Entity, Entity entity)
    {
        if (entity.HasComponent<ComponentType>())
            n_Entity["Entity"].push_back(SaveComponent(entity.GetComponent<ComponentType>()));
    }

    template <typename ComponentType> YAML::Node SceneLoader::SaveComponent(ComponentType component)
    {
        YAML::Node n_Component;
        n_Component["UnknownComponent"] = DemangleName<ComponentType>();
        return n_Component;
    }
    template <> YAML::Node SceneLoader::SaveComponent(Tag component)
    {
        YAML::Node n_Component;
        n_Component["Tag"] = component.tag;
        return n_Component;
    }
    template <> YAML::Node SceneLoader::SaveComponent(Id component)
    {
        // TODO: Save UUID in hex format.
        YAML::Node n_Component;
        n_Component["UUID"] = (uint64_t)component;
        return n_Component;
    }
    template <> YAML::Node SceneLoader::SaveComponent(Transformation component)
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
    template <> YAML::Node SceneLoader::SaveComponent(RenderModel component)
    {
        YAML::Node n_Component;

        m_State.m_ModelsPath.insert(Relative(component.path));
        n_Component["RenderModel"] = Relative(component.path);

        return n_Component;
    }
    template <> YAML::Node SceneLoader::SaveComponent(Ref<Shader> component)
    {
        YAML::Node n_Component;

        m_State.m_Shaders.insert(component);
        n_Component["Shader"] = component->GetName();

        return n_Component;
    }
    template <> YAML::Node SceneLoader::SaveComponent(UniqueShader component)
    {
        YAML::Node n_Component;

        m_State.m_Shaders.insert(component);
        n_Component["UniqueShader"] = component.shader->GetName();

        return n_Component;
    }
    template <> YAML::Node SceneLoader::SaveComponent(FPSCamera component)
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
    template <> YAML::Node SceneLoader::SaveComponent(LightSource component)
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

    void SceneLoader::Save(Ref<Scene> scene, Path path)
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
            auto entity = scene->GetByUUID(UUID(uuid));
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

    //*~~~Loading~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    Vec2 GetVec2(YAML::Node node)
    {
        Vec2 res;

        res.x = node[0].as<float>();
        res.y = node[1].as<float>();

        return res;
    }
    Vec3 GetVec3(YAML::Node node)
    {
        Vec3 res;

        res.x = node[0].as<float>();
        res.y = node[1].as<float>();
        res.z = node[2].as<float>();

        return res;
    }
    Vec4 GetVec4(YAML::Node node)
    {
        Vec4 res;

        res.x = node[0].as<float>();
        res.y = node[1].as<float>();
        res.z = node[2].as<float>();
        res.w = node[3].as<float>();

        return res;
    }

    template <> void SceneLoader::LoadComponent<Tag>(YAML::Node n_Component, Entity& entity)
    {
        entity.AddComponent<Tag>(n_Component.as<std::string>());
    }
    template <> void SceneLoader::LoadComponent<Id>(YAML::Node n_Component, Entity& entity)
    {
        entity.AddComponent<Id>(Id(UUID(n_Component.as<uint64_t>())));
    }
    template <> void SceneLoader::LoadComponent<Transformation>(YAML::Node n_Component, Entity& entity)
    {
        Transformation transformation;

        transformation.translation = GetVec3(n_Component["Translation"]);
        transformation.translation_offset = GetVec3(n_Component["TranslationOffset"]);
        transformation.rotation = GetVec3(n_Component["Rotation"]);
        transformation.rotation_offet = GetVec3(n_Component["RotationOffset"]);
        transformation.scale = GetVec3(n_Component["Scale"]);
        transformation.scale_offset = GetVec3(n_Component["ScaleOffset"]);

        entity.AddComponent<Transformation>(transformation);
    }
    template <> void SceneLoader::LoadComponent<FPSCamera>(YAML::Node n_Component, Entity& entity)
    {
        FPSCamera fps_camera;

        fps_camera.SetFov(n_Component["FOV"].as<float>());
        fps_camera.SetAspect(n_Component["Aspect"].as<float>());
        fps_camera.SetNearPlane(n_Component["NearPlane"].as<float>());
        fps_camera.SetFarPlane(n_Component["FarPlane"].as<float>());
        fps_camera.SetPos(GetVec3(n_Component["Position"]));
        fps_camera.SetDir(GetVec3(n_Component["Direction"]));

        entity.AddComponent<FPSCamera>(fps_camera);
    }
    template <> void SceneLoader::LoadComponent<LightSource>(YAML::Node n_Component, Entity& entity)
    {
        LightSource light_source;

        light_source.type = StringToLightSourceType(n_Component["Type"].as<std::string>());
        light_source.ambient = GetVec3(n_Component["Ambient"]);
        light_source.diffuse = GetVec3(n_Component["Diffuse"]);
        light_source.specular = GetVec3(n_Component["Specular"]);
        light_source.direction = GetVec3(n_Component["Direction"]);
        light_source.position = GetVec3(n_Component["Position"]);
        light_source.clq = GetVec3(n_Component["CLQ"]);
        light_source.inner_cone_cos = n_Component["InnerCone"].as<float>();
        light_source.outer_cone_cos = n_Component["OuterCone"].as<float>();

        entity.AddComponent<LightSource>(light_source);
    }

    void SceneLoader::LoadShaders(YAML::Node n_Shaders)
    {
        for (const auto& n_Shader : n_Shaders)
        {
            std::vector<ShaderPart> parts;
            for (const auto& n_Part : n_Shader.second)
            {
                ShaderPart shader_part;
                shader_part.type = StringToShaderPartType(n_Part.first.as<std::string>());
                shader_part.path =
                    fs::canonical(Config::GetPath(DE_CFG_EXECUTABLE_PATH) / n_Part.second.as<std::string>());
                parts.push_back(shader_part);
            }
            Ref<Shader> shader = Shader::Create(n_Shader.first.as<std::string>(), parts);
        }
    }
    void SceneLoader::LoadModels(YAML::Node n_Models)
    {
        for (const auto& path : n_Models)
        {
            ModelLoader::Load(Config::GetPath(DE_CFG_EXECUTABLE_PATH) / path.as<std::string>());
        }
    }
    void SceneLoader::LoadAssets(YAML::Node n_Assets)
    {
        LoadShaders(n_Assets["Shaders"]);
        LoadModels(n_Assets["Models"]);
    }
    void SceneLoader::LoadEntity(YAML::Node n_Entity, Ref<Scene> scene)
    {
        YAML::Node n_Components = n_Entity["Entity"];

        Entity entity = scene->CreateEmptyEntity();
        for (const auto& component : n_Components)
        {
            std::string name = (*component.begin()).first.as<std::string>();
            YAML::Node component_data = (*component.begin()).second.as<YAML::Node>();

            if (name == "Tag") LoadComponent<Tag>(component_data, entity);
            if (name == "UUID") LoadComponent<Id>(component_data, entity);
            if (name == "Transformation") LoadComponent<Transformation>(component_data, entity);
            if (name == "FPSCamera") LoadComponent<FPSCamera>(component_data, entity);
            if (name == "LightSource") LoadComponent<LightSource>(component_data, entity);
        }
        scene->UpdateEmptyEntity(entity);
    }
    void SceneLoader::LoadEntities(YAML::Node n_Entities, Ref<Scene> scene)
    {
        for (const auto& entity : n_Entities)
        {
            LoadEntity(entity, scene);
        }
    }

    void SceneLoader::Load(Ref<Scene> scene, Path path)
    {
        YAML::Node n_Root, n_Scene, n_Assets, n_Entities;

        n_Root = YAML::LoadFile(path.string());
        n_Scene = n_Root["Scene"];
        n_Assets = n_Scene["Assets"];
        n_Entities = n_Scene["Entities"];

        LoadAssets(n_Assets);
        LoadEntities(n_Entities, scene);
    }
}  // namespace DE