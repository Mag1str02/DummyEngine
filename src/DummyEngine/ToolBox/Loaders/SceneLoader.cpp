#include "Addition/Base.h"
#include "Core/Objects/LightSources/LightSource.h"
#include "Core/ECS/Entity.hpp"
#include "ToolBox/Loaders/SceneLoader.h"
#include "ToolBox/Loaders/ModelLoader.h"

namespace DE
{
    SceneLoader::LoaderState SceneLoader::m_State;

    //*~~~Additional~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    void SceneLoader::LoaderState::Clear()
    {
        m_SShaders.clear();
        m_SModels.clear();
        m_LShaders.clear();
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

    template <typename ComponentType> void SceneLoader::SaveComponent(YAML::Node n_Entity, Entity entity)
    {
        // TODO: Different names for different unknown components...
        if (entity.HasComponent<ComponentType>()) n_Entity["UnknownComponent"] = DemangleName<ComponentType>();
    }
    template <> void SceneLoader::SaveComponent<Tag>(YAML::Node n_Entity, Entity entity)
    {
        if (entity.HasComponent<Tag>()) n_Entity["Tag"] = entity.GetComponent<Tag>().tag;
    }
    template <> void SceneLoader::SaveComponent<Id>(YAML::Node n_Entity, Entity entity)
    {
        // TODO: Save UUID in hex format.
        if (entity.HasComponent<Id>()) n_Entity["UUID"] = (uint64_t)entity.GetComponent<Id>();
    }
    template <> void SceneLoader::SaveComponent<Transformation>(YAML::Node n_Entity, Entity entity)
    {
        if (entity.HasComponent<Transformation>())
        {
            n_Entity["Transformation"]["Translation"] = NodeVec3(entity.GetComponent<Transformation>().translation);
            n_Entity["Transformation"]["TranslationOffset"] = NodeVec3(entity.GetComponent<Transformation>().translation_offset);
            n_Entity["Transformation"]["Rotation"] = NodeVec3(entity.GetComponent<Transformation>().rotation);
            n_Entity["Transformation"]["RotationOffset"] = NodeVec3(entity.GetComponent<Transformation>().rotation_offet);
            n_Entity["Transformation"]["Scale"] = NodeVec3(entity.GetComponent<Transformation>().scale);
            n_Entity["Transformation"]["ScaleOffset"] = NodeVec3(entity.GetComponent<Transformation>().scale_offset);
        }
    }
    template <> void SceneLoader::SaveComponent<RenderModel>(YAML::Node n_Entity, Entity entity)
    {
        if (entity.HasComponent<RenderModel>())
        {
            n_Entity["RenderModel"] = entity.GetComponent<RenderModel>().GetName();
        }
    }
    template <> void SceneLoader::SaveComponent<Ref<Shader>>(YAML::Node n_Entity, Entity entity)
    {
        if (entity.HasComponent<Ref<Shader>>())
        {
            n_Entity["Shader"] = entity.GetComponent<Ref<Shader>>()->GetName();
        }
    }
    template <> void SceneLoader::SaveComponent<FPSCamera>(YAML::Node n_Entity, Entity entity)
    {
        if (entity.HasComponent<FPSCamera>())
        {
            n_Entity["FPSCamera"]["FOV"] = entity.GetComponent<FPSCamera>().m_FOV;
            n_Entity["FPSCamera"]["Aspect"] = entity.GetComponent<FPSCamera>().m_Aspect;
            n_Entity["FPSCamera"]["NearPlane"] = entity.GetComponent<FPSCamera>().m_NearPlane;
            n_Entity["FPSCamera"]["FarPlane"] = entity.GetComponent<FPSCamera>().m_FarPlane;
            n_Entity["FPSCamera"]["Position"] = NodeVec3(entity.GetComponent<FPSCamera>().m_Position);
            n_Entity["FPSCamera"]["Direction"] = NodeVec3(entity.GetComponent<FPSCamera>().m_Direction);
        }
    }
    template <> void SceneLoader::SaveComponent<LightSource>(YAML::Node n_Entity, Entity entity)
    {
        if (entity.HasComponent<LightSource>())
        {
            n_Entity["LightSource"]["Type"] = LightSourceTypeToString(entity.GetComponent<LightSource>().type);
            n_Entity["LightSource"]["Ambient"] = NodeVec3(entity.GetComponent<LightSource>().ambient);
            n_Entity["LightSource"]["Diffuse"] = NodeVec3(entity.GetComponent<LightSource>().diffuse);
            n_Entity["LightSource"]["Specular"] = NodeVec3(entity.GetComponent<LightSource>().specular);
            n_Entity["LightSource"]["Direction"] = NodeVec3(entity.GetComponent<LightSource>().direction);
            n_Entity["LightSource"]["Position"] = NodeVec3(entity.GetComponent<LightSource>().position);
            n_Entity["LightSource"]["CLQ"] = NodeVec3(entity.GetComponent<LightSource>().clq);
            n_Entity["LightSource"]["InnerCone"] = entity.GetComponent<LightSource>().inner_cone_cos;
            n_Entity["LightSource"]["OuterCone"] = entity.GetComponent<LightSource>().outer_cone_cos;
        }
    }

    void SceneLoader::SaveEntity(YAML::Node n_Entity, Entity entity)
    {
        SaveComponent<Tag>(n_Entity, entity);
        SaveComponent<Id>(n_Entity, entity);
        SaveComponent<RenderModel>(n_Entity, entity);
        SaveComponent<Transformation>(n_Entity, entity);
        SaveComponent<Ref<Shader>>(n_Entity, entity);
        SaveComponent<FPSCamera>(n_Entity, entity);
        SaveComponent<LightSource>(n_Entity, entity);
    }
    YAML::Node SceneLoader::SaveEntities(Ref<Scene> scene)
    {
        YAML::Node n_Entities;

        std::vector<std::pair<Tag, Entity>> entities;

        for (auto [uuid, entity_id] : scene->m_EntityByUUID)
        {
            auto entity = scene->GetByUUID(UUID(uuid));
            entities.push_back({entity.GetComponent<Tag>(), entity});
        }

        std::sort(entities.begin(), entities.end(), [](const auto& a, const auto& b) { return a.first.tag < b.first.tag; });

        int cnt = 0;
        for (auto [id, entity] : entities)
        {
            if (entity.HasComponent<UniqueShader>())
            {
                m_State.m_SShaders.push_back(entity);
            }
            else if (entity.HasComponent<ModelOwner>())
            {
                m_State.m_SModels.push_back(entity);
            }
            else
            {
                SaveEntity(n_Entities["Entity_" + std::to_string(cnt++)], entity);
            }
        }
        return n_Entities;
    }
    YAML::Node SceneLoader::SaveModels()
    {
        YAML::Node n_Models;

        for (auto model : m_State.m_SModels)
        {
            YAML::Node n_Model;
            n_Models[(std::string)model.GetComponent<Tag>()] = n_Model;

            n_Model["Path"] = RelativeToExecutable(model.GetComponent<ModelOwner>().data->path.string()).string();
            n_Model["Compress"] = model.GetComponent<ModelOwner>().properties.compress;
            n_Model["FlipUV"] = model.GetComponent<ModelOwner>().properties.flip_uvs;
        }
        return n_Models;
    }
    YAML::Node SceneLoader::SaveShaders()
    {
        YAML::Node n_Shaders;

        for (auto entity : m_State.m_SShaders)
        {
            auto shader = entity.GetComponent<UniqueShader>().shader;
            for (const auto& part : shader->GetParts())
            {
                n_Shaders[shader->GetName()][ShaderPartTypeToString(part.type)] = RelativeToExecutable(part.path).string();
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
        n_Scene["Entities"] = SaveEntities(scene);

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

    template <typename ComponentType> void SceneLoader::LoadComponent(YAML::Node n_Component, Entity& entity)
    {
        Logger::Warning("Loading", "SceneLoader", "Load function of " + DemangleName<ComponentType>() + " undefined.");
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
    template <> void SceneLoader::LoadComponent<RenderModel>(YAML::Node n_Component, Entity& entity)
    {
        entity.AddComponent<RenderModel>(m_State.m_Scene->GetByName(n_Component.as<std::string>()).GetComponent<ModelOwner>().render_model);
        // std::cout << "Adding model component: " << n_Component.as<std::string>() << std::endl;
    }
    template <> void SceneLoader::LoadComponent<Ref<Shader>>(YAML::Node n_Component, Entity& entity)
    {
        entity.AddComponent<Ref<Shader>>(m_State.m_LShaders[n_Component.as<std::string>()]);
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
                shader_part.path = fs::canonical(Config::GetPath(DE_CFG_EXECUTABLE_PATH) / n_Part.second.as<std::string>());
                parts.push_back(shader_part);
            }
            Ref<Shader> shader = Shader::Create(n_Shader.first.as<std::string>(), parts);

            m_State.m_Scene->CreateEntity(n_Shader.first.as<std::string>()).AddComponent<UniqueShader>().shader = shader;
            m_State.m_LShaders[n_Shader.first.as<std::string>()] = shader;
        }
    }
    void SceneLoader::LoadModels(YAML::Node n_Models)
    {
        for (const auto& node : n_Models)
        {
            Entity entity = m_State.m_Scene->CreateEntity(node.first.as<std::string>());
            ModelOwner& model = entity.AddComponent<ModelOwner>();
            model.properties.compress = node.second["Compress"].as<bool>();
            model.properties.flip_uvs = node.second["FlipUV"].as<bool>();
            model.data = ModelLoader::Load(Config::GetPath(DE_CFG_EXECUTABLE_PATH) / node.second["Path"].as<std::string>(), model.properties);
            model.render_model.FillData(model.data, node.first.as<std::string>());
        }
    }
    void SceneLoader::LoadAssets(YAML::Node n_Assets)
    {
        LoadShaders(n_Assets["Shaders"]);
        LoadModels(n_Assets["Models"]);
    }
    void SceneLoader::LoadEntity(YAML::Node n_Entity)
    {
        Entity entity = m_State.m_Scene->CreateEmptyEntity();

        if (n_Entity["Tag"]) LoadComponent<Tag>(n_Entity["Tag"], entity);
        if (n_Entity["UUID"]) LoadComponent<Id>(n_Entity["UUID"], entity);
        if (n_Entity["Transformation"]) LoadComponent<Transformation>(n_Entity["Transformation"], entity);
        if (n_Entity["RenderModel"]) LoadComponent<RenderModel>(n_Entity["RenderModel"], entity);
        if (n_Entity["Shader"]) LoadComponent<Ref<Shader>>(n_Entity["Shader"], entity);
        if (n_Entity["FPSCamera"]) LoadComponent<FPSCamera>(n_Entity["FPSCamera"], entity);
        if (n_Entity["LightSource"]) LoadComponent<LightSource>(n_Entity["LightSource"], entity);

        m_State.m_Scene->UpdateEmptyEntity(entity);
    }
    void SceneLoader::LoadEntities(YAML::Node n_Entities)
    {
        for (const auto& entity : n_Entities)
        {
            LoadEntity(entity.second);
        }
    }

    void SceneLoader::Load(Ref<Scene> scene, Path path)
    {
        YAML::Node n_Root, n_Scene, n_Assets, n_Entities;

        m_State.Clear();
        m_State.m_Scene = scene;

        n_Root = YAML::LoadFile(path.string());
        n_Scene = n_Root["Scene"];
        n_Assets = n_Scene["Assets"];
        n_Entities = n_Scene["Entities"];

        LoadAssets(n_Assets);
        LoadEntities(n_Entities);

        m_State.m_Scene = nullptr;
    }
}  // namespace DE