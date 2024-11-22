#pragma once

#include "DummyEngine/Utils/Helpers/Singleton.h"
#include "DummyEngine/Utils/Types/Types.h"

namespace DummyEngine {

    class Config : public Singleton<Config> {
        SINGLETON(Config)

        struct Fields {
            U32 MaxShaderCompileErrorLen = 512;
            U32 MaxInputFrameAmount      = 100;
            U32 MaxMessageTypeLength     = 7;

            Path ExecutablePath;
            Path ShaderPath;
            Path ModelPath;
            Path AssetPath;
            Path LogPath;
            Path TexturePath;
            Path CachePath;
            Path ScriptPath;
            Path ScriptCachePath;
            Path FontPath;
            Path ScenePath;

            API RendererAPI;
        };

    public:
        S_METHOD_DEF(const Fields&, Get, ());

        S_METHOD_DEF(Path, RelativeToExecutable, (const Path& path));

    private:
        Fields fields_;
    };

    Path RelativeToExecutable(const Path& path);
}  // namespace DummyEngine