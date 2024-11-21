#pragma once

#include "DummyEngine/Core/ResourceManaging/Assets.h"
#include "DummyEngine/Core/Scene/Scene.h"
#include "DummyEngine/Core/Scene/SceneRenderer.h"

#include <yaml-cpp/yaml.h>

namespace DummyEngine {

    struct SceneAssets {
        std::vector<ScriptAsset>     Scripts;
        std::vector<RenderMeshAsset> RenderMeshes;
        std::vector<TextureAsset>    Textures;
        std::vector<ShaderAsset>     Shaders;
    };

    struct SceneFileData {
        SceneRenderer::GraphicsSettings Settings;
        SceneAssets                     Assets;
        YAML::Node                      Hierarchy;
        std::string                     Name;
    };

    class SceneLoader {
        LOG_AUTHOR(SceneLoader)
    public:
        static std::optional<SceneFileData> LoadScene(const Path& path);
        static bool                         SaveScene(const SceneFileData& data, const Path& path);
        static Ref<Scene>                   Serialize(const YAML::Node& hierarchy);
        static YAML::Node                   Deserialize(Ref<Scene> scene);
    };

}  // namespace DummyEngine