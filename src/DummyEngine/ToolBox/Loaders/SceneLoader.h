#pragma once

// clang-format off
#include "DummyEngine/Utils/Base.h"
#include "DummyEngine/Core/Scene/Components.h"
#include "DummyEngine/Core/Scene/Scene.h"

#include <yaml-cpp/yaml.h>
// clang-format on

namespace DE {
    struct SceneAssets {
        std::vector<ScriptAsset>     scripts;
        std::vector<RenderMeshAsset> render_meshes;
        std::vector<TextureAsset>    textures;
        std::vector<ShaderAsset>     shaders;
    };

    struct SceneSerializationData {
        SceneAssets assets;
        YAML::Node  hierarchy_node;
        std::string name;
    };

    class SceneLoader {
    public:
        static void                   Save(Ref<Scene> scene, const Path& path);
        static Ref<Scene>             Instantiate(const SceneSerializationData& data);
        static SceneSerializationData LoadSerializationData(const Path& path);

    private:
        template <typename ComponentType> static void SaveComponent(YAML::Node& n_Entity, Entity entity);
        template <typename ComponentType> static void LoadComponent(Ref<Scene> scene, YAML::Node n_Component, Entity& entity);

        static YAML::Node SaveModels();
        static YAML::Node SaveScripts();
        static YAML::Node SaveTextures();
        static YAML::Node SaveShaders();
        static YAML::Node SaveAssets();
        static YAML::Node SaveNode(Ref<SceneHierarchyNode> node);
        static void       SaveEntity(YAML::Node& n_Entities, Entity entity);

        static void   LoadHierarchyNode(Ref<Scene> scene, YAML::Node n_Array, Ref<SceneHierarchyNode> load_to);
        static Entity LoadEntity(Ref<Scene> scene, YAML::Node n_Entity);

        static void SLoadAssets(SceneAssets& data, const YAML::Node& assets);
    };

}  // namespace DE