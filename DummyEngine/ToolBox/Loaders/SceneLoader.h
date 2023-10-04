#pragma once

// clang-format off
#include "DummyEngine/Utils/Base.h"
#include "DummyEngine/Core/ResourceManaging/Assets.h"
#include "DummyEngine/Core/Scene/Scene.h"
#include "DummyEngine/Core/Scene/SceneRenderer.h"
#include "DummyEngine/Core/Threading/Task.hpp"

#include <yaml-cpp/yaml.h>
// clang-format on

namespace DE {
    struct SceneAssets {
        std::vector<ScriptAsset>     scripts;
        std::vector<RenderMeshAsset> render_meshes;
        std::vector<TextureAsset>    textures;
        std::vector<ShaderAsset>     shaders;
    };

    struct SceneFileData {
        SceneRenderer::Settings settings;
        SceneAssets             assets;
        YAML::Node              hierarchy;
        std::string             name;
    };

    class SceneLoader {
    public:
        static std::optional<SceneFileData> LoadScene(const Path& path);
        static bool                         SaveScene(const SceneFileData& data, const Path& path);
        static Ref<Scene>                   Serialize(const YAML::Node& hierarchy);
        static YAML::Node                   Deserialize(Ref<Scene> scene);
    };

    template <typename Component> class LoadComponentTask : public Task {
    private:
        Ref<Scene> m_scene;
        Entity    m_entity;

    public:
        YAML::Node n_component;
        LoadComponentTask(Ref<Scene> scene, YAML::Node node, Entity entity) : m_scene(scene), n_component(node), m_entity(entity) {
            LOG_INFO("LoadComponentTask", typeid(Component).name());
        }
        void Run() override;
    };

}  // namespace DE