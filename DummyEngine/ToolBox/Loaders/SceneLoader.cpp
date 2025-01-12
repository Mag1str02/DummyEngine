#include "DummyEngine/ToolBox/Loaders/SceneLoader.h"

#include "DummyEngine/Core/Application/Config.h"
#include "DummyEngine/Core/ECS/ECS.h"
#include "DummyEngine/Core/Objects/LightSources/LightSource.h"
#include "DummyEngine/Core/Physics/PhysicsComponent.h"
#include "DummyEngine/Core/ResourceManaging/AssetManager.h"
#include "DummyEngine/Core/ResourceManaging/ResourceManager.h"
#include "DummyEngine/Core/Scene/Components.h"
#include "DummyEngine/Core/Scene/SceneHierarchy.h"
#include "DummyEngine/Core/Scene/SceneRenderer.h"
#include "DummyEngine/Core/Scripting/ScriptEngine.h"
#include "DummyEngine/ToolBox/Loaders/TextureLoader.h"

namespace DummyEngine {

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
            case ScriptFieldType::Entity:
                if (field.Get<Entity>().Valid()) {
                    return YAML::Node(field.Get<Entity>().Get<IDComponent>().Hex());
                } else {
                    return YAML::Node();
                }
                break;
            default: return YAML::Node();
        }
    }

    template <typename ComponentType> void SaveComponent(YAML::Node&, Entity) {
        // TODO: Different names for different unknown components...
        DE_ASSERT(false, "Trying to save unknown component");
    }
    template <> void SaveComponent<TagComponent>(YAML::Node& n_entity, Entity entity) {
        if (entity.Has<TagComponent>()) {
            n_entity["Tag"] = entity.Get<TagComponent>().Tag;
        }
    }
    template <> void SaveComponent<IDComponent>(YAML::Node& n_entity, Entity entity) {
        if (entity.Has<IDComponent>()) {
            n_entity["UUID"] = entity.Get<IDComponent>().Hex();
        }
    }
    template <> void SaveComponent<TransformComponent>(YAML::Node& n_entity, Entity entity) {
        if (entity.Has<TransformComponent>()) {
            n_entity["Transformation"]["Translation"]       = NodeVec3(entity.Get<TransformComponent>().Translation);
            n_entity["Transformation"]["TranslationOffset"] = NodeVec3(entity.Get<TransformComponent>().TranslationOffset);
            n_entity["Transformation"]["Rotation"]          = NodeVec3(entity.Get<TransformComponent>().Rotation);
            n_entity["Transformation"]["RotationOffset"]    = NodeVec3(entity.Get<TransformComponent>().RotationOffet);
            n_entity["Transformation"]["Scale"]             = NodeVec3(entity.Get<TransformComponent>().Scale);
            n_entity["Transformation"]["ScaleOffset"]       = NodeVec3(entity.Get<TransformComponent>().ScaleOffset);
        }
    }
    template <> void SaveComponent<RenderMeshComponent>(YAML::Node& n_entity, Entity entity) {
        if (entity.Has<RenderMeshComponent>()) {
            n_entity["RenderModel"]["UUID"] = entity.Get<RenderMeshComponent>().ID.Hex();
            auto materials                  = n_entity["RenderModel"]["Materials"];
            U32  cnt                        = 0;
            for (const auto& mesh : entity.Get<RenderMeshComponent>()->GetSubMeshes()) {
                YAML::Node mat;
                mat["Type"]             = MaterialTypeToStr(mesh.Material.Type);
                mat["Albedo"]           = NodeVec3(mesh.Material.Albedo);
                mat["Ambient"]          = NodeVec3(mesh.Material.Ambient);
                mat["ORM"]              = NodeVec3(mesh.Material.ORM);
                mat["Diffuse"]          = NodeVec3(mesh.Material.Diffuse);
                mat["Specular"]         = NodeVec3(mesh.Material.Specular);
                mat["Emission"]         = NodeVec3(mesh.Material.Emission);
                mat["Shininess"]        = mesh.Material.Shininess;
                mat["EmissionStrength"] = mesh.Material.EmissionStrength;
                mat["MeshID"]           = cnt++;
                materials.push_back(mat);
            }
        }
    }
    template <> void SaveComponent<ShaderComponent>(YAML::Node& n_entity, Entity entity) {
        if (entity.Has<ShaderComponent>()) {
            n_entity["Shader"] = entity.Get<ShaderComponent>().ID.Hex();
        }
    }
    template <> void SaveComponent<FPSCamera>(YAML::Node& n_entity, Entity entity) {
        if (entity.Has<FPSCamera>()) {
            n_entity["FPSCamera"]["FOV"]       = entity.Get<FPSCamera>().FOV;
            n_entity["FPSCamera"]["Aspect"]    = entity.Get<FPSCamera>().Aspect;
            n_entity["FPSCamera"]["NearPlane"] = entity.Get<FPSCamera>().NearPlane;
            n_entity["FPSCamera"]["FarPlane"]  = entity.Get<FPSCamera>().FarPlane;
            n_entity["FPSCamera"]["Position"]  = NodeVec3(entity.Get<FPSCamera>().Position);
            n_entity["FPSCamera"]["Direction"] = NodeVec3(entity.Get<FPSCamera>().Direction);
        }
    }
    template <> void SaveComponent<LightSource>(YAML::Node& n_entity, Entity entity) {
        if (entity.Has<LightSource>()) {
            n_entity["LightSource"]["Type"]      = LightSourceTypeToString(entity.Get<LightSource>().Type);
            n_entity["LightSource"]["Ambient"]   = NodeVec3(entity.Get<LightSource>().Ambient);
            n_entity["LightSource"]["Diffuse"]   = NodeVec3(entity.Get<LightSource>().Diffuse);
            n_entity["LightSource"]["Specular"]  = NodeVec3(entity.Get<LightSource>().Specular);
            n_entity["LightSource"]["Direction"] = NodeVec3(entity.Get<LightSource>().Direction);
            n_entity["LightSource"]["Position"]  = NodeVec3(entity.Get<LightSource>().Position);
            n_entity["LightSource"]["CLQ"]       = NodeVec3(entity.Get<LightSource>().CLQ);
            n_entity["LightSource"]["InnerCone"] = entity.Get<LightSource>().InnerConeCosinus;
            n_entity["LightSource"]["OuterCone"] = entity.Get<LightSource>().OuterConeCosinus;
        }
    }
    template <> void SaveComponent<SkyBoxComponent>(YAML::Node& n_entity, Entity entity) {
        if (entity.Has<SkyBoxComponent>()) {
            auto skybox                = entity.Get<SkyBoxComponent>();
            n_entity["SkyBox"]["Type"] = (skybox.Type == SkyBoxComponent::TexType::CubeMap ? "CubeMap" : "Equirectangular");
            n_entity["SkyBox"]["UUID"] = skybox.ID.Hex();
        }
    }
    template <> void SaveComponent<ScriptComponent>(YAML::Node& n_entity, Entity entity) {
        if (entity.Has<ScriptComponent>() && entity.Get<ScriptComponent>().Valid()) {
            auto& script_instance      = entity.Get<ScriptComponent>();
            n_entity["Script"]["UUID"] = script_instance.ID().Hex();
            for (auto [name, field] : *script_instance) {
                n_entity["Script"]["Fields"][name.get()] = NodeScriptField(field);
            }
        }
    }
    template <> void SaveComponent<Physics::PhysicsComponent>(YAML::Node& n_entity, Entity entity) {
        if (entity.Has<Physics::PhysicsComponent>()) {
            const auto& phys                  = entity.Get<Physics::PhysicsComponent>();
            n_entity["Physics"]["InvMass"]    = phys.InvertedMass;
            n_entity["Physics"]["InvInertia"] = phys.InvertedInertia;
            n_entity["Physics"]["Collidable"] = phys.Collidable;
            n_entity["Physics"]["Gravity"]    = phys.Gravity;
        }
    }

    void SaveEntity(YAML::Node& n_entity, Entity entity) {
        SaveComponent<TagComponent>(n_entity, entity);
        SaveComponent<IDComponent>(n_entity, entity);
        SaveComponent<TransformComponent>(n_entity, entity);
        SaveComponent<RenderMeshComponent>(n_entity, entity);
        SaveComponent<ShaderComponent>(n_entity, entity);
        SaveComponent<FPSCamera>(n_entity, entity);
        SaveComponent<LightSource>(n_entity, entity);
        SaveComponent<SkyBoxComponent>(n_entity, entity);
        SaveComponent<ScriptComponent>(n_entity, entity);
        SaveComponent<Physics::PhysicsComponent>(n_entity, entity);
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

    YAML::Node SaveRendererSettings(const SceneRenderer::GraphicsSettings& settings) {
        YAML::Node set;
        set["Bloom"]                   = settings.Bloom;
        set["BloomThreshold"]          = settings.BloomThreshold;
        set["BloomSoftTreshold"]       = settings.BloomSoftThreshold;
        set["BloomDepth"]              = settings.BloomDepth;
        set["BloomRadius"]             = settings.BloomRadius;
        set["BloomStrength"]           = settings.BloomStrength;
        set["GammaToneMapping"]        = settings.GammaToneMapping;
        set["Exposure"]                = settings.Exposure;
        set["Gamma"]                   = settings.Gamma;
        set["UseDirectionalShadowMap"] = settings.UseDirectionalShadowMap;
        set["UsePointShadows"]         = settings.UsePointShadows;
        return set;
    }
    YAML::Node SaveTextures(const SceneAssets& assets) {
        YAML::Node n_textures;
        for (const auto& texture : assets.Textures) {
            YAML::Node n_texture;
            n_textures[texture.Name] = n_texture;
            n_texture["Path"]        = Config::RelativeToExecutable(texture.LoadingProps.Path).generic_string();
            n_texture["FlipUV"]      = texture.LoadingProps.FlipUV;
            n_texture["UUID"]        = texture.ID.Hex();
        }
        return n_textures;
    }
    YAML::Node SaveModels(const SceneAssets& assets) {
        YAML::Node n_models;
        for (const auto& model : assets.RenderMeshes) {
            YAML::Node n_model;

            n_models[model.Name] = n_model;
            n_model["Path"]      = Config::RelativeToExecutable(model.LoadingProps.Path).generic_string();
            n_model["Compress"]  = model.LoadingProps.Compress;
            n_model["FlipUV"]    = model.LoadingProps.FlipUV;
            n_model["UUID"]      = model.ID.Hex();
        }
        return n_models;
    }
    YAML::Node SaveScripts(const SceneAssets& assets) {
        YAML::Node n_scripts;
        for (const auto& script : assets.Scripts) {
            YAML::Node n_script;
            n_scripts[script.Name] = n_script;
            n_script["Path"]       = Config::RelativeToExecutable(script.Path).generic_string();
            n_script["UUID"]       = script.ID.Hex();
        }
        return n_scripts;
    }
    YAML::Node SaveShaders(const SceneAssets& assets) {
        YAML::Node n_shaders;
        for (const auto& shader : assets.Shaders) {
            YAML::Node n_shader;
            n_shaders[shader.Name] = n_shader;
            n_shader["UUID"]       = shader.ID.Hex();
            for (const auto& part : shader.Parts) {
                n_shader[ShaderPartTypeToString(part.Type)] = Config::RelativeToExecutable(part.Path).generic_string();
            }
        }
        return n_shaders;
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
            // case ScriptFieldType::Entity: field.Get<Entity>() = GetVec4(node); break;
            default: break;
        }
    }

    template <typename ComponentType> void LoadComponent(Ref<Scene>, YAML::Node, Entity&) {
        LOG_WARNING("Load function of {} undefined", DemangledName<ComponentType>());
    }
    template <> void LoadComponent<TagComponent>(Ref<Scene>, YAML::Node n_component, Entity& entity) {
        entity.AddComponent<TagComponent>(n_component.as<std::string>());
    }
    template <> void LoadComponent<IDComponent>(Ref<Scene>, YAML::Node n_component, Entity& entity) {
        auto s = n_component.as<std::string>();
        entity.AddComponent<IDComponent>(IDComponent(UUID(n_component.as<std::string>())));
    }
    template <> void LoadComponent<TransformComponent>(Ref<Scene>, YAML::Node n_component, Entity& entity) {
        TransformComponent transformation;

        transformation.Translation       = GetVec3(n_component["Translation"]);
        transformation.TranslationOffset = GetVec3(n_component["TranslationOffset"]);
        transformation.Rotation          = GetVec3(n_component["Rotation"]);
        transformation.RotationOffet     = GetVec3(n_component["RotationOffset"]);
        transformation.Scale             = GetVec3(n_component["Scale"]);
        transformation.ScaleOffset       = GetVec3(n_component["ScaleOffset"]);

        entity.AddComponent(transformation);
    }
    template <> void LoadComponent<RenderMeshComponent>(Ref<Scene>, YAML::Node n_component, Entity& entity) {
        UUID id = n_component["UUID"].as<std::string>();
        if (!ResourceManager::HasRenderMesh(id) && !ResourceManager::LoadRenderMesh(id)) {
            LOG_WARNING("RenderMesh {} not found in ResourceManager", id);
        }
        if (!ResourceManager::HasHitBox(id) && !ResourceManager::LoadHitBox(id)) {
            LOG_WARNING("Failed to load HitBox {}", id);
        }
        auto& meshes = entity.AddComponent<RenderMeshComponent>({id, ResourceManager::GetRenderMesh(id).value()->Copy()})->Mesh->GetSubMeshes();

        for (const auto& mat : n_component["Materials"]) {
            U32   mesh_id             = mat["MeshID"].as<U32>();
            auto& material            = meshes[mesh_id].Material;
            material.Albedo           = GetVec3(mat["Albedo"]);
            material.Ambient          = GetVec3(mat["Ambient"]);
            material.Diffuse          = GetVec3(mat["Diffuse"]);
            material.Specular         = GetVec3(mat["Specular"]);
            material.Emission         = GetVec3(mat["Emission"]);
            material.ORM              = GetVec3(mat["ORM"]);
            material.Shininess        = mat["Shininess"].as<float>();
            material.EmissionStrength = mat["EmissionStrength"].as<float>();
            material.Type             = MaterialTypeFromStr(mat["Type"].as<std::string>());
        }
    }
    template <> void LoadComponent<ShaderComponent>(Ref<Scene> scene, YAML::Node n_component, Entity& entity) {
        UUID id = n_component.as<std::string>();
        if (!ResourceManager::HasShader(id) && !ResourceManager::LoadShader(id)) {
            LOG_WARNING("Shader {} not found in ResourceManager", id);
        } else {
            entity.AddComponent<ShaderComponent>({id, ResourceManager::GetShader(id).value()});
            scene->GetRenderer()->RequestShader(id);
        }
    }
    template <> void LoadComponent<FPSCamera>(Ref<Scene>, YAML::Node n_component, Entity& entity) {
        FPSCamera fps_camera;

        fps_camera.SetFov(n_component["FOV"].as<float>());
        fps_camera.SetAspect(n_component["Aspect"].as<float>());
        fps_camera.SetNearPlane(n_component["NearPlane"].as<float>());
        fps_camera.SetFarPlane(n_component["FarPlane"].as<float>());
        fps_camera.SetPos(GetVec3(n_component["Position"]));
        fps_camera.SetDir(GetVec3(n_component["Direction"]));

        entity.AddComponent<FPSCamera>(fps_camera);
    }
    template <> void LoadComponent<LightSource>(Ref<Scene>, YAML::Node n_component, Entity& entity) {
        LightSource light_source;

        light_source.Type             = StringToLightSourceType(n_component["Type"].as<std::string>());
        light_source.Ambient          = GetVec3(n_component["Ambient"]);
        light_source.Diffuse          = GetVec3(n_component["Diffuse"]);
        light_source.Specular         = GetVec3(n_component["Specular"]);
        light_source.Direction        = GetVec3(n_component["Direction"]);
        light_source.Position         = GetVec3(n_component["Position"]);
        light_source.CLQ              = GetVec3(n_component["CLQ"]);
        light_source.InnerConeCosinus = n_component["InnerCone"].as<float>();
        light_source.OuterConeCosinus = n_component["OuterCone"].as<float>();

        entity.AddComponent<LightSource>(light_source);
    }
    template <> void LoadComponent<SkyBoxComponent>(Ref<Scene>, YAML::Node n_component, Entity& entity) {
        UUID                     id = n_component["UUID"].as<std::string>();
        SkyBoxComponent::TexType type =
            (n_component["Type"].as<std::string>() == "CubeMap" ? SkyBoxComponent::TexType::CubeMap : SkyBoxComponent::TexType::Equirectangular);
        if (type == SkyBoxComponent::TexType::CubeMap) {
            if (!ResourceManager::HasCubeMap(id) && !ResourceManager::LoadCubeMap(id)) {
                LOG_WARNING("CubeMap {} not found in ResourceManager", id);
            } else {
                Ref<SkyBox> skybox = CreateRef<SkyBox>(ResourceManager::GetCubeMap(id).value());
                entity.AddComponent<SkyBoxComponent>({type, id, skybox});
            }
        } else {
            auto asset = AssetManager::GetTextureAsset(id);
            if (!asset.has_value()) {
                LOG_WARNING("CubeMap {} not found in ResourceManager", id);
            } else {
                auto        asset  = AssetManager::GetTextureAsset(id);
                Ref<SkyBox> skybox = CreateRef<SkyBox>(TextureLoader::Load(asset.value().LoadingProps));
                entity.AddComponent<SkyBoxComponent>({type, id, skybox});
            }
        }
    }
    template <> void LoadComponent<ScriptComponent>(Ref<Scene>, YAML::Node n_component, Entity& entity) {
        ScriptComponent script = ScriptEngine::CreateScript(n_component["UUID"].as<std::string>());
        entity.AddComponent<ScriptComponent>(script);
        if (script.Valid()) {
            for (auto [name, field] : *script) {
                if (n_component["Fields"][name.get()]) {
                    GetField(n_component["Fields"][name.get()], field);
                }
            }
        } else {
            LOG_INFO("Failed to create valid script: {}", script.ID());
        }
    }
    template <> void LoadComponent<Physics::PhysicsComponent>(Ref<Scene>, YAML::Node n_component, Entity& entity) {
        Physics::PhysicsComponent component{Vec3(0, 0, 0),
                                            Vec3(0, 0, 0),
                                            n_component["InvMass"].as<float>(),
                                            n_component["InvInertia"].as<float>(),
                                            n_component["Collidable"].as<bool>(),
                                            n_component["Gravity"].as<bool>(),
                                            Vec3(0, 0, 0),
                                            Vec3(0, 0, 0)};
        entity.AddComponent<Physics::PhysicsComponent>(component);
    }

    Entity LoadEntity(Ref<Scene> scene, YAML::Node n_entity) {
        Entity entity = scene->CreateEmptyEntity();

        if (n_entity["Tag"]) LoadComponent<TagComponent>(scene, n_entity["Tag"], entity);
        if (n_entity["UUID"]) LoadComponent<IDComponent>(scene, n_entity["UUID"], entity);
        if (n_entity["Transformation"]) LoadComponent<TransformComponent>(scene, n_entity["Transformation"], entity);
        if (n_entity["RenderModel"]) LoadComponent<RenderMeshComponent>(scene, n_entity["RenderModel"], entity);
        if (n_entity["Shader"]) LoadComponent<ShaderComponent>(scene, n_entity["Shader"], entity);
        if (n_entity["FPSCamera"]) LoadComponent<FPSCamera>(scene, n_entity["FPSCamera"], entity);
        if (n_entity["LightSource"]) LoadComponent<LightSource>(scene, n_entity["LightSource"], entity);
        if (n_entity["SkyBox"]) LoadComponent<SkyBoxComponent>(scene, n_entity["SkyBox"], entity);
        if (n_entity["Script"]) LoadComponent<ScriptComponent>(scene, n_entity["Script"], entity);
        if (n_entity["Physics"]) LoadComponent<Physics::PhysicsComponent>(scene, n_entity["Physics"], entity);
        return entity;
    }
    void LoadHierarchyNode(Ref<Scene> scene, YAML::Node n_array, SceneHierarchy::Node load_to) {
        for (const auto& node : n_array) {
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
                asset.ID   = node.second["UUID"].as<std::string>();
                asset.Name = node.first.as<std::string>();
                asset.Path = Config::Get().ExecutablePath / node.second["Path"].as<std::string>();
                data.Scripts.emplace_back(std::move(asset));
            }
        }
        //*RenderMeshAssets
        {
            for (const auto& node : assets["Models"]) {
                RenderMeshAsset asset;
                asset.Name                  = node.first.as<std::string>();
                asset.ID                    = node.second["UUID"].as<std::string>();
                asset.LoadingProps.Compress = node.second["Compress"].as<bool>();
                asset.LoadingProps.FlipUV   = node.second["FlipUV"].as<bool>();
                std::string type            = (node.second["MatType"] ? node.second["MatType"].as<std::string>() : "");
                asset.LoadingProps.Path     = Config::Get().ExecutablePath / node.second["Path"].as<std::string>();
                data.RenderMeshes.emplace_back(std::move(asset));
            }
        }
        //*TextureAssets
        {
            for (const auto& node : assets["Textures"]) {
                TextureAsset asset;
                asset.ID                  = node.second["UUID"].as<std::string>();
                asset.Name                = node.first.as<std::string>();
                asset.LoadingProps.FlipUV = node.second["FlipUV"].as<bool>();
                asset.LoadingProps.Path   = Config::Get().ExecutablePath / node.second["Path"].as<std::string>();
                data.Textures.emplace_back(std::move(asset));
            }
        }
        //*ShaderAssets
        {
            for (const auto& n_shader : assets["Shaders"]) {
                ShaderAsset asset;
                asset.ID   = n_shader.second["UUID"].as<std::string>();
                asset.Name = n_shader.first.as<std::string>();
                for (const auto& n_part : n_shader.second) {
                    if (n_part.first.as<std::string>() != "UUID") {
                        ShaderPart shader_part;
                        shader_part.Type = StringToShaderPartType(n_part.first.as<std::string>());
                        shader_part.Path = fs::canonical(Config::Get().ExecutablePath / n_part.second.as<std::string>());
                        asset.Parts.push_back(shader_part);
                    }
                }
                data.Shaders.emplace_back(std::move(asset));
            }
        }
    }
    void LoadRendererSettings(SceneRenderer::GraphicsSettings& settings, YAML::Node node) {
        settings.Bloom              = node["Bloom"].as<bool>();
        settings.BloomThreshold     = node["BloomThreshold"].as<float>();
        settings.BloomSoftThreshold = node["BloomSoftTreshold"].as<float>();
        settings.BloomDepth         = node["BloomDepth"].as<U32>();
        settings.BloomRadius        = node["BloomRadius"].as<float>();
        settings.BloomStrength      = node["BloomStrength"].as<float>();
        settings.GammaToneMapping   = node["GammaToneMapping"].as<bool>();
        settings.Exposure           = node["Exposure"].as<float>();
        settings.Gamma              = node["Gamma"].as<float>();
        if (node["UseDirectionalShadowMap"].IsDefined()) {
            settings.UseDirectionalShadowMap = node["UseDirectionalShadowMap"].as<bool>();
        }
    }

    //*~~~SceneLoader~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    std::optional<SceneFileData> SceneLoader::LoadScene(const Path& path) {
        try {
            SceneFileData result;
            YAML::Node    n_scene = YAML::LoadFile(path.string())["Scene"];
            LoadAssets(result.Assets, n_scene["Assets"]);
            LoadRendererSettings(result.Settings, n_scene["RendererSettings"]);
            result.Hierarchy = n_scene["Hierarchy"];
            result.Name      = n_scene["Name"].as<std::string>();
            LOG_INFO("Loaded SceneData for {}", Config::RelativeToExecutable(path));
            return result;
        } catch (const std::exception& e) {
            LOG_ERROR("Failed to load SceneData for {} due to exception: {}", path.string(), e.what());
            return {};
        }
    }
    Ref<Scene> SceneLoader::Serialize(const YAML::Node& hierarchy) {
        try {
            Ref<Scene> scene = CreateRef<Scene>();
            LoadHierarchyNode(scene, hierarchy, scene->GetHierarchyRoot());
            LOG_INFO("Serialized scene");
            return scene;
        } catch (const std::exception& e) {
            LOG_ERROR("Failed to serialize scene due to exception: {}", e.what());
        } catch (...) {
            LOG_ERROR("Failed to serialize scene");
        }
        return nullptr;
    }
    YAML::Node SceneLoader::Deserialize(Ref<Scene> scene) {
        return SaveNode(scene->GetHierarchyRoot());
    }
    bool SceneLoader::SaveScene(const SceneFileData& data, const Path& path) {
        try {
            YAML::Node    n_root, n_scene;
            std::ofstream output_file;

            output_file.open(path);
            if (!output_file.is_open()) {
                LOG_ERROR("Failed to open file {} to save scene", Config::RelativeToExecutable(path));
                return false;
            }

            n_root["Scene"]             = n_scene;
            n_scene["Name"]             = data.Name;
            n_scene["Assets"]           = SaveAssets(data.Assets);
            n_scene["Hierarchy"]        = data.Hierarchy;
            n_scene["RendererSettings"] = SaveRendererSettings(data.Settings);
            output_file << n_root;
            LOG_INFO("Saved scene {} to file {}", data.Name, Config::RelativeToExecutable(path));
            return true;
        } catch (...) {
            LOG_ERROR("Failed to open file {} to save scene", Config::RelativeToExecutable(path));
            return false;
        }
    }

}  // namespace DummyEngine