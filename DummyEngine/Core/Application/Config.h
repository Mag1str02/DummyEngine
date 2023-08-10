#pragma once

#include "DummyEngine/Utils/Base.h"

namespace DE {

#define DE_CFG_MAX_COMPILE_ERROR_LEN 1
#define DE_CFG_MAX_INPUT_FRAME_AMOUNT 2

#define DE_CFG_DEFAULT_CAMERA_FOV 1
#define DE_CFG_DEFAULT_CAMERA_ASPECT 2
#define DE_CFG_DEFAULT_CAMERA_NEAR_PLANE 3
#define DE_CFG_DEFAULT_CAMERA_FAR_PLANE 4

#define DE_CFG_EXECUTABLE_PATH 1
#define DE_CFG_ASSET_PATH 2
#define DE_CFG_LOG_PATH 3
#define DE_CFG_MODEL_PATH 4
#define DE_CFG_SHADER_PATH 5
#define DE_CFG_TEXTURE_PATH 6
#define DE_CFG_SCRIPT_PATH 7
#define DE_CFG_FONT_PATH 8
#define DE_CFG_SCENE_PATH 11

#define DE_CFG_CACHE_PATH 9
#define DE_CFG_SCRIPT_CACHE_PATH 10

#define DE_CFG_RENDER_API 1

    // TODO: Singleton

    class Config {
    public:
        static U32 GetI(U32 setting);
        static float    GetF(U32 setting);
        static Path     GetPath(U32 setting);

        static API GetRenderAPI();

        static void Initialize();
        static void Terminate();

    private:
        struct Configs {
            U32 c_MaxShaderCompileErrorLen = 512;
            U32 c_MaxInputFrameAmount      = 100;
            U32 c_MaxMessageTypeLength     = 7;

            float c_DefaultCameraFOV       = 90.0f;
            float c_DefaultCameraAspect    = 16.0f / 9.0f;
            float c_DefaultCameraNearPlane = 0.1f;
            float c_DefaultCameraFarPlane  = 2'000'000.0f;

            Path c_ExecutablePath;
            Path c_ShaderPath;
            Path c_ModelPath;
            Path c_AssetPath;
            Path c_LogPath;
            Path c_TexturePath;
            Path c_CachePath;
            Path c_ScriptPath;
            Path c_ScriptCachePath;
            Path c_FontPath;
            Path c_ScenePath;

            API c_RenderAPI;
        };
        static Configs m_Configs;
    };

    Path RelativeToExecutable(const Path& path);
}  // namespace DE