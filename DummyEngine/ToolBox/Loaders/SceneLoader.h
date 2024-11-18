#pragma once

// clang-format off
#include "DummyEngine/Utils/Base.h"
#include "DummyEngine/Core/ResourceManaging/Assets.h"
#include "DummyEngine/Core/Scene/Scene.h"
#include "DummyEngine/Core/Scene/SceneRenderer.h"

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
        LOGGER_AUTHOR(SceneLoader)
    public:
        static std::optional<SceneFileData> LoadScene(const Path& path);
        static bool                         SaveScene(const SceneFileData& data, const Path& path);
        static Ref<Scene>                   Serialize(const YAML::Node& hierarchy);
        static YAML::Node                   Deserialize(Ref<Scene> scene);
    };

}  // namespace DE