#include "Config.h"

#include <iostream>

namespace DummyEngine {

    SINGLETON_BASE(Config);

    S_INITIALIZE() {
        fields_.ExecutablePath = fs::current_path();
        std::cout << "Executable path: " << fields_.ExecutablePath.string() << std::endl;

        fields_.AssetPath       = fields_.ExecutablePath / "Assets";
        fields_.LogPath         = fields_.ExecutablePath / "Logs";
        fields_.ModelPath       = fields_.AssetPath / "Models";
        fields_.ShaderPath      = fields_.AssetPath / "Shaders";
        fields_.TexturePath     = fields_.AssetPath / "Textures";
        fields_.ScriptPath      = fields_.AssetPath / "Scripts";
        fields_.CachePath       = fields_.ExecutablePath / "Cache";
        fields_.ScriptCachePath = fields_.CachePath / "Scripts";
        fields_.FontPath        = fields_.AssetPath / "Fonts";
        fields_.ScenePath       = fields_.AssetPath / "Scenes";

        fields_.RendererAPI = API::OpenGL;

        return Unit();
    }
    S_TERMINATE() {
        return Unit();
    }

    S_METHOD_IMPL(const Config::Fields&, Get, (), ()) {
        return fields_;
    }

    S_METHOD_IMPL(Path, RelativeToExecutable, (const Path& path), (path)) {
        return fs::relative(path, fields_.ExecutablePath);
    }
}  // namespace DummyEngine