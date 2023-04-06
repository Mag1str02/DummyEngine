#include "DummyEngine/ToolBox/Loaders/SceneLoader.h"

#include "DummyEngine/Core/Application/Config.h"
#include "DummyEngine/Core/ECS/ECS.h"
#include "DummyEngine/Core/Objects/LightSources/LightSource.h"
#include "DummyEngine/Core/ResourceManaging/ResourceManager.h"
#include "DummyEngine/Core/Scene/SceneHierarchy.h"
#include "DummyEngine/Core/Scene/SceneRenderData.h"
#include "DummyEngine/Core/Scripting/ScriptEngine.h"
#include "DummyEngine/ToolBox/Loaders/ModelLoader.h"
#include "DummyEngine/ToolBox/Loaders/TextureLoader.h"
#include "DummyEngine/Utils/Base.h"

namespace DE {

    //*~~~Saving~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    YAML::Node NodeVec2(Vec2 vec) {
        YAML::Node res = YAML::Load("[]");

        res.push_back(vec.x);
        res.push_back(vec.y);

        return res;
    }
    YAML::Node NodeVec3(Vec3 vec) {
        YAML::Node res = YAML::Load("[]");

        res.push_back(vec.x);
        res.push_back(vec.y);
        res.push_back(vec.z);

        return res;
    }
    YAML::Node NodeVec4(Vec4 vec) {
        YAML::Node res = YAML::Load("[]");

        res.push_back(vec.x);
        res.push_back(vec.y);
        res.push_back(vec.z);
        res.push_back(vec.w);

        return res;
    }

    YAML::Node NodeScriptField(Script::Field& field) {
        switch (field.GetType()) {
            case ScriptFieldType::Float: return YAML::Node(field.Get<float>());
            case ScriptFieldType::Double: return YAML::Node(field.Get<double>());
            case ScriptFieldType::Bool: return YAML::Node(field.Get<bool>());
            case ScriptFieldType::String: return YAML::Node(field.Get<std::string>());
            case ScriptFieldType::S32: return YAML::Node(field.Get<S32>());
            case ScriptFieldType::S64: return YAML::Node(field.Get<S64>());
            case ScriptFieldType::U32: return YAML::Node(field.Get<U32>());
            case ScriptFieldType::U64: return YAML::Node(field.Get<U64>());
            case ScriptFieldType::Vec2: return NodeVec2(field.Get<Vec2>());
            case ScriptFieldType::Vec3: return NodeVec3(field.Get<Vec3>());
            case ScriptFieldType::Vec4: return NodeVec4(field.Get<Vec4>());
            default: return YAML::Node();
        }
    }

    template <typename ComponentType> void SaveComponent(YAML::Node& n_Entity, Entity entity) {
        // TODO: Different names for different unknown components...
        DE_ASSERT(false, "Trying to save unknown component");
    }
    template <> void SaveComponent<TagComponent>(YAML::Node& n_Entity, Entity entity) {
        if (entity.Has<TagComponent>()) {
            n_Entity["Tag"] = entity.Get<TagComponent>().tag;
        }
    }
    template <> void SaveComponent<IDComponent>(YAML::Node& n_Entity, Entity entity) {
        if (entity.Has<IDComponent>()) {
            n_Entity["UUID"] = entity.Get<IDComponent>().Hex();
        }
    }
    template <> void SaveComponent<TransformComponent>(YAML::Node& n_Entity, Entity entity) {
        if (entity.Has<TransformComponent>()) {
            n_Entity["Transformation"]["Translation"]       = NodeVec3(entity.Get<TransformComponent>().translation);
            n_Entity["Transformation"]["TranslationOffset"] = NodeVec3(entity.Get<TransformComponent>().translation_offset);
            n_Entity["Transformation"]["Rotation"]          = NodeVec3(entity.Get<TransformComponent>().rotation);
            n_Entity["Transformation"]["RotationOffset"]    = NodeVec3(entity.Get<TransformComponent>().rotation_offet);
            n_Entity["Transformation"]["Scale"]             = NodeVec3(entity.Get<TransformComponent>().scale);
            n_Entity["Transformation"]["ScaleOffset"]       = NodeVec3(entity.Get<TransformComponent>().scale_offset);
        }
    }
    template <> void SaveComponent<RenderMeshComponent>(YAML::Node& n_Entity, Entity entity) {
        if (entity.Has<RenderMeshComponent>()) {
            n_Entity["RenderModel"] = entity.Get<RenderMeshComponent>().id.Hex();
        }
    }
    template <> void SaveComponent<ShaderComponent>(YAML::Node& n_Entity, Entity entity) {
        if (entity.Has<ShaderComponent>()) {
            n_Entity["Shader"] = entity.Get<ShaderComponent>().id.Hex();
        }
    }
    template <> void SaveComponent<FPSCamera>(YAML::Node& n_Entity, Entity entity) {
        if (entity.Has<FPSCamera>()) {
            n_Entity["FPSCamera"]["FOV"]       = entity.Get<FPSCamera>().m_FOV;
            n_Entity["FPSCamera"]["Aspect"]    = entity.Get<FPSCamera>().m_Aspect;
            n_Entity["FPSCamera"]["NearPlane"] = entity.Get<FPSCamera>().m_NearPlane;
            n_Entity["FPSCamera"]["FarPlane"]  = entity.Get<FPSCamera>().m_FarPlane;
            n_Entity["FPSCamera"]["Position"]  = NodeVec3(entity.Get<FPSCamera>().m_Position);
            n_Entity["FPSCamera"]["Direction"] = NodeVec3(entity.Get<FPSCamera>().m_Direction);
        }
    }
    template <> void SaveComponent<LightSource>(YAML::Node& n_Entity, Entity entity) {
        if (entity.Has<LightSource>()) {
            n_Entity["LightSource"]["Type"]      = LightSourceTypeToString(entity.Get<LightSource>().type);
            n_Entity["LightSource"]["Ambient"]   = NodeVec3(entity.Get<LightSource>().ambient);
            n_Entity["LightSource"]["Diffuse"]   = NodeVec3(entity.Get<LightSource>().diffuse);
            n_Entity["LightSource"]["Specular"]  = NodeVec3(entity.Get<LightSource>().specular);
            n_Entity["LightSource"]["Direction"] = NodeVec3(entity.Get<LightSource>().direction);
            n_Entity["LightSource"]["Position"]  = NodeVec3(entity.Get<LightSource>().position);
            n_Entity["LightSource"]["CLQ"]       = NodeVec3(entity.Get<LightSource>().clq);
            n_Entity["LightSource"]["InnerCone"] = entity.Get<LightSource>().inner_cone_cos;
            n_Entity["LightSource"]["OuterCone"] = entity.Get<LightSource>().outer_cone_cos;
        }
    }
    template <> void SaveComponent<SkyBox>(YAML::Node& n_Entity, Entity entity) {
        if (entity.Has<SkyBox>()) {
            n_Entity["SkyBox"] = entity.Get<SkyBox>().id.Hex();
        }
    }
    template <> void SaveComponent<ScriptComponent>(YAML::Node& n_Entity, Entity entity) {
        if (entity.Has<ScriptComponent>() && entity.Get<ScriptComponent>().Valid()) {
            auto& script_instance      = entity.Get<ScriptComponent>();
            n_Entity["Script"]["UUID"] = script_instance.ID().Hex();
            for (auto [name, field] : *script_instance) {
                n_Entity["Script"]["Fields"][name.get()] = NodeScriptField(field);
            }
        }
    }

    void SaveEntity(YAML::Node& n_Entity, Entity entity) {
        SaveComponent<TagComponent>(n_Entity, entity);
        SaveComponent<IDComponent>(n_Entity, entity);
        SaveComponent<TransformComponent>(n_Entity, entity);
        SaveComponent<RenderMeshComponent>(n_Entity, entity);
        SaveComponent<ShaderComponent>(n_Entity, entity);
        SaveComponent<FPSCamera>(n_Entity, entity);
        SaveComponent<LightSource>(n_Entity, entity);
        SaveComponent<SkyBox>(n_Entity, entity);
        SaveComponent<ScriptComponent>(n_Entity, entity);
    }
    YAML::Node SaveNode(SceneHierarchy::Node node) {
        YAML::Node res;
        if (node.IsEntity()) {
            YAML::Node entity = res["Entity"];
            SaveEntity(entity, node.GetEntity());
        }
        if (node.IsFolder()) {
            YAML::Node childs;
            if (node.GetParent().Valid()) {
                res["Node"]["Name"] = node.GetName();
                childs              = res["Node"]["Childs"];
            } else {
                childs = res;
            }
            for (auto child : node.GetChilds()) {
                childs.push_back(SaveNode(child));
            }
        }
        return res;
    }

    YAML::Node SaveTextures(const SceneAssets& assets) {
        YAML::Node n_Textures;
        for (const auto& texture : assets.textures) {
            YAML::Node n_Texture;
            n_Textures[texture.name] = n_Texture;
            n_Texture["Path"]        = RelativeToExecutable(texture.loading_props.path).string();
            n_Texture["FlipUV"]      = texture.loading_props.flip_uvs;
            n_Texture["UUID"]        = texture.id.Hex();
        }
        return n_Textures;
    }
    YAML::Node SaveModels(const SceneAssets& assets) {
        YAML::Node n_Models;
        for (const auto& model : assets.render_meshes) {
            YAML::Node n_Model;

            n_Models[model.name] = n_Model;
            n_Model["Path"]      = RelativeToExecutable(model.loading_props.path).string();
            n_Model["Compress"]  = model.loading_props.compress;
            n_Model["FlipUV"]    = model.loading_props.flip_uvs;
            n_Model["UUID"]      = model.id.Hex();
        }
        return n_Models;
    }
    YAML::Node SaveScripts(const SceneAssets& assets) {
        YAML::Node n_Scripts;
        for (const auto& script : assets.scripts) {
            YAML::Node n_Script;
            n_Scripts[script.name] = n_Script;
            n_Script["Path"]       = RelativeToExecutable(script.path).string();
            n_Script["UUID"]       = script.id.Hex();
        }
        return n_Scripts;
    }
    YAML::Node SaveShaders(const SceneAssets& assets) {
        YAML::Node n_Shaders;
        for (const auto& shader : assets.shaders) {
            YAML::Node n_Shader;
            n_Shaders[shader.name] = n_Shader;
            n_Shader["UUID"]       = shader.id.Hex();
            for (const auto& part : shader.parts) {
                n_Shader[ShaderPartTypeToString(part.type)] = RelativeToExecutable(part.path).string();
            }
        }
        return n_Shaders;
    }
    YAML::Node SaveAssets(const SceneAssets& assets) {
        YAML::Node res;
        res["Models"]   = SaveModels(assets);
        res["Textures"] = SaveTextures(assets);
        res["Shaders"]  = SaveShaders(assets);
        res["Scripts"]  = SaveScripts(assets);
        return res;
    }

    //*~~~Loading~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    Vec2 GetVec2(YAML::Node node) {
        Vec2 res;

        res.x = node[0].as<float>();
        res.y = node[1].as<float>();

        return res;
    }
    Vec3 GetVec3(YAML::Node node) {
        Vec3 res;

        res.x = node[0].as<float>();
        res.y = node[1].as<float>();
        res.z = node[2].as<float>();

        return res;
    }
    Vec4 GetVec4(YAML::Node node) {
        Vec4 res;

        res.x = node[0].as<float>();
        res.y = node[1].as<float>();
        res.z = node[2].as<float>();
        res.w = node[3].as<float>();

        return res;
    }
    void GetField(YAML::Node node, Script::Field field) {
        switch (field.GetType()) {
            case ScriptFieldType::Float: field.Get<float>() = node.as<float>(); break;
            case ScriptFieldType::Double: field.Get<double>() = node.as<double>(); break;
            case ScriptFieldType::Bool: field.Get<bool>() = node.as<bool>(); break;
            case ScriptFieldType::String: field.Get<std::string>() = node.as<std::string>(); break;
            case ScriptFieldType::S32: field.Get<S32>() = node.as<S32>(); break;
            case ScriptFieldType::S64: field.Get<S64>() = node.as<S64>(); break;
            case ScriptFieldType::U32: field.Get<U32>() = node.as<U32>(); break;
            case ScriptFieldType::U64: field.Get<U64>() = node.as<U64>(); break;
            case ScriptFieldType::Vec2: field.Get<Vec2>() = GetVec2(node); break;
            case ScriptFieldType::Vec3: field.Get<Vec3>() = GetVec3(node); break;
            case ScriptFieldType::Vec4: field.Get<Vec4>() = GetVec4(node); break;
            default: break;
        }
    }

    template <typename ComponentType> void LoadComponent(Ref<Scene> scene, YAML::Node n_Component, Entity& entity) {
        LOG_WARNING("SceneLoader", "Load function of ", DemangledName<ComponentType>(), " undefined.");
    }
    template <> void LoadComponent<TagComponent>(Ref<Scene> scene, YAML::Node n_Component, Entity& entity) {
        entity.AddComponent<TagComponent>(n_Component.as<std::string>());
    }
    template <> void LoadComponent<IDComponent>(Ref<Scene> scene, YAML::Node n_Component, Entity& entity) {
        auto s = n_Component.as<std::string>();
        entity.AddComponent<IDComponent>(IDComponent(UUID(n_Component.as<std::string>())));
    }
    template <> void LoadComponent<TransformComponent>(Ref<Scene> scene, YAML::Node n_Component, Entity& entity) {
        TransformComponent transformation;

        transformation.translation        = GetVec3(n_Component["Translation"]);
        transformation.translation_offset = GetVec3(n_Component["TranslationOffset"]);
        transformation.rotation           = GetVec3(n_Component["Rotation"]);
        transformation.rotation_offet     = GetVec3(n_Component["RotationOffset"]);
        transformation.scale              = GetVec3(n_Component["Scale"]);
        transformation.scale_offset       = GetVec3(n_Component["ScaleOffset"]);

        entity.AddComponent(transformation);
    }
    template <> void LoadComponent<RenderMeshComponent>(Ref<Scene> scene, YAML::Node n_Component, Entity& entity) {
        UUID id = n_Component.as<std::string>();
        if (!ResourceManager::HasRenderMesh(id) && !ResourceManager::LoadRenderMesh(id)) {
            LOG_WARNING("SceneLoader", "RenderMesh (", id, ") not found in ResourceManager");
        }
        entity.AddComponent<RenderMeshComponent>({id, nullptr});
    }
    template <> void LoadComponent<ShaderComponent>(Ref<Scene> scene, YAML::Node n_Component, Entity& entity) {
        UUID id = n_Component.as<std::string>();
        if (!ResourceManager::HasShader(id) && !ResourceManager::LoadShader(id)) {
            LOG_WARNING("SceneLoader", "Shader (", id, ") not found in ResourceManager");
        } else {
            entity.AddComponent<ShaderComponent>({id, ResourceManager::GetShader(id).value()});
            scene->GetRenderData()->RequestShader(id);
        }
    }
    template <> void LoadComponent<FPSCamera>(Ref<Scene> scene, YAML::Node n_Component, Entity& entity) {
        FPSCamera fps_camera;

        fps_camera.SetFov(n_Component["FOV"].as<float>());
        fps_camera.SetAspect(n_Component["Aspect"].as<float>());
        fps_camera.SetNearPlane(n_Component["NearPlane"].as<float>());
        fps_camera.SetFarPlane(n_Component["FarPlane"].as<float>());
        fps_camera.SetPos(GetVec3(n_Component["Position"]));
        fps_camera.SetDir(GetVec3(n_Component["Direction"]));

        entity.AddComponent<FPSCamera>(fps_camera);
    }
    template <> void LoadComponent<LightSource>(Ref<Scene> scene, YAML::Node n_Component, Entity& entity) {
        LightSource light_source;

        light_source.type           = StringToLightSourceType(n_Component["Type"].as<std::string>());
        light_source.ambient        = GetVec3(n_Component["Ambient"]);
        light_source.diffuse        = GetVec3(n_Component["Diffuse"]);
        light_source.specular       = GetVec3(n_Component["Specular"]);
        light_source.direction      = GetVec3(n_Component["Direction"]);
        light_source.position       = GetVec3(n_Component["Position"]);
        light_source.clq            = GetVec3(n_Component["CLQ"]);
        light_source.inner_cone_cos = n_Component["InnerCone"].as<float>();
        light_source.outer_cone_cos = n_Component["OuterCone"].as<float>();

        entity.AddComponent<LightSource>(light_source);
    }
    template <> void LoadComponent<SkyBox>(Ref<Scene> scene, YAML::Node n_Component, Entity& entity) {
        UUID id = n_Component.as<std::string>();
        if (!ResourceManager::HasCubeMap(id) && !ResourceManager::LoadCubeMap(id)) {
            LOG_WARNING("SceneLoader", "CubeMap (", id, ") not found in ResourceManager");
        } else {
            entity.AddComponent<SkyBox>({id, ResourceManager::GetCubeMap(id).value()});
        }
    }
    template <> void LoadComponent<ScriptComponent>(Ref<Scene> scene, YAML::Node n_Component, Entity& entity) {
        ScriptComponent script = ScriptEngine::CreateScript(n_Component["UUID"].as<std::string>());
        entity.AddComponent<ScriptComponent>(script);
        if (script.Valid()) {
            for (auto [name, field] : *script) {
                if (n_Component["Fields"][name.get()]) {
                    GetField(n_Component["Fields"][name.get()], field);
                }
            }
        } else {
            LOG_INFO("SceneLoader", "Failed to create valid script: ", script.ID());
        }
    }

    Entity LoadEntity(Ref<Scene> scene, YAML::Node n_Entity) {
        Entity entity = scene->CreateEmptyEntity();

        if (n_Entity["Tag"]) LoadComponent<TagComponent>(scene, n_Entity["Tag"], entity);
        if (n_Entity["UUID"]) LoadComponent<IDComponent>(scene, n_Entity["UUID"], entity);
        if (n_Entity["Transformation"]) LoadComponent<TransformComponent>(scene, n_Entity["Transformation"], entity);
        if (n_Entity["RenderModel"]) LoadComponent<RenderMeshComponent>(scene, n_Entity["RenderModel"], entity);
        if (n_Entity["Shader"]) LoadComponent<ShaderComponent>(scene, n_Entity["Shader"], entity);
        if (n_Entity["FPSCamera"]) LoadComponent<FPSCamera>(scene, n_Entity["FPSCamera"], entity);
        if (n_Entity["LightSource"]) LoadComponent<LightSource>(scene, n_Entity["LightSource"], entity);
        if (n_Entity["SkyBox"]) LoadComponent<SkyBox>(scene, n_Entity["SkyBox"], entity);
        if (n_Entity["Script"]) LoadComponent<ScriptComponent>(scene, n_Entity["Script"], entity);

        if (entity.Has<RenderMeshComponent>() && entity.Has<ShaderComponent>()) {
            UUID mesh_id   = entity.Get<RenderMeshComponent>().id;
            UUID shader_id = entity.Get<ShaderComponent>().id;

            entity.Get<RenderMeshComponent>().mesh_instance = scene->GetRenderData()->GetRenderMeshInstance(mesh_id, shader_id);
        }
        return entity;
    }
    void LoadHierarchyNode(Ref<Scene> scene, YAML::Node n_Array, SceneHierarchy::Node load_to) {
        for (const auto& node : n_Array) {
            if (node["Entity"]) {
                Entity entity = LoadEntity(scene, node["Entity"]);
                load_to.AddEntity(entity);
            }
            if (node["Node"]) {
                auto folder = load_to.AddFolder(node["Node"]["Name"].as<std::string>());
                LoadHierarchyNode(scene, node["Node"]["Childs"], folder);
            }
        }
    }
    void LoadAssets(SceneAssets& data, const YAML::Node& assets) {
        //*ScriptAssets
        {
            for (const auto& node : assets["Scripts"]) {
                ScriptAsset asset;
                asset.id   = node.second["UUID"].as<std::string>();
                asset.name = node.first.as<std::string>();
                asset.path = Config::GetPath(DE_CFG_EXECUTABLE_PATH) / node.second["Path"].as<std::string>();
                data.scripts.emplace_back(std::move(asset));
            }
        }
        //*RenderMeshAssets
        {
            for (const auto& node : assets["Models"]) {
                RenderMeshAsset asset;
                asset.name                   = node.first.as<std::string>();
                asset.id                     = node.second["UUID"].as<std::string>();
                asset.loading_props.compress = node.second["Compress"].as<bool>();
                asset.loading_props.flip_uvs = node.second["FlipUV"].as<bool>();
                asset.loading_props.path     = Config::GetPath(DE_CFG_EXECUTABLE_PATH) / node.second["Path"].as<std::string>();
                data.render_meshes.emplace_back(std::move(asset));
            }
        }
        //*TextureAssets
        {
            for (const auto& node : assets["Textures"]) {
                TextureAsset asset;
                asset.id                     = node.second["UUID"].as<std::string>();
                asset.name                   = node.first.as<std::string>();
                asset.loading_props.flip_uvs = node.second["FlipUV"].as<bool>();
                asset.loading_props.path     = Config::GetPath(DE_CFG_EXECUTABLE_PATH) / node.second["Path"].as<std::string>();
                data.textures.emplace_back(std::move(asset));
            }
        }
        //*ShaderAssets
        {
            for (const auto& n_Shader : assets["Shaders"]) {
                ShaderAsset asset;
                asset.id   = n_Shader.second["UUID"].as<std::string>();
                asset.name = n_Shader.first.as<std::string>();
                for (const auto& n_Part : n_Shader.second) {
                    if (n_Part.first.as<std::string>() != "UUID") {
                        ShaderPart shader_part;
                        shader_part.type = StringToShaderPartType(n_Part.first.as<std::string>());
                        shader_part.path = fs::canonical(Config::GetPath(DE_CFG_EXECUTABLE_PATH) / n_Part.second.as<std::string>());
                        asset.parts.push_back(shader_part);
                    }
                }
                data.shaders.emplace_back(std::move(asset));
            }
        }
    }

    //*~~~SceneLoader~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    std::optional<SceneFileData> SceneLoader::LoadScene(const Path& path) {
        try {
            SceneFileData result;
            YAML::Node    n_Scene = YAML::LoadFile(path.string())["Scene"];
            LoadAssets(result.assets, n_Scene["Assets"]);
            result.hierarchy = n_Scene["Hierarchy"];
            result.name      = n_Scene["Name"].as<std::string>();
            LOG_INFO("SceneLoader", "Loaded SceneData for (", RelativeToExecutable(path), ")");
            return result;
        } catch (...) {
            LOG_ERROR("SceneLoader", "Failed to load SceneData for (", path.string(), ")");
            return {};
        }
    }
    Ref<Scene> SceneLoader::Serialize(const YAML::Node& hierarchy) {
        try {
            Ref<Scene> scene = CreateRef<Scene>();
            LoadHierarchyNode(scene, hierarchy, scene->GetHierarchyRoot());
            LOG_INFO("SceneLoader", "Serialized scene");
            return scene;
        } catch (...) {
            LOG_ERROR("SceneLoader", "Failed to serialize scene");
            return nullptr;
        }
    }
    YAML::Node SceneLoader::Deserialize(Ref<Scene> scene) {
        return SaveNode(scene->GetHierarchyRoot());
    }
    bool SceneLoader::SaveScene(const SceneFileData& data, const Path& path) {
        try {
            YAML::Node    n_Root, n_Scene;
            std::ofstream output_file;

            output_file.open(path);
            if (!output_file.is_open()) {
                LOG_ERROR("SceneLoader", "Failed to open file (", RelativeToExecutable(path), ") to save scene");
                return false;
            }

            n_Root["Scene"]      = n_Scene;
            n_Scene["Name"]      = data.name;
            n_Scene["Assets"]    = SaveAssets(data.assets);
            n_Scene["Hierarchy"] = data.hierarchy;
            output_file << n_Root;
            LOG_INFO("SceneLoader", "Saved scene (", data.name, ") to file (", RelativeToExecutable(path), ")");
            return true;
        } catch (...) {
            LOG_ERROR("SceneLoader", "Failed to open file (", RelativeToExecutable(path), ") to save scene");
            return false;
        }
    }

}  // namespace DE