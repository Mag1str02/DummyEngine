#include "DummyEngine/Core/Application/Config.h"

namespace DE {
    Config::Configs Config::m_Configs;

    U32 Config::GetI(U32 setting) {
        switch (setting) {
            case DE_CFG_MAX_COMPILE_ERROR_LEN: return m_Configs.c_MaxShaderCompileErrorLen;
            case DE_CFG_MAX_INPUT_FRAME_AMOUNT: return m_Configs.c_MaxInputFrameAmount;
            default: DE_ASSERT(false, "Wrong config requested (", setting, ")");
        }
        return 0;
    }
    float Config::GetF(U32 setting) {
        switch (setting) {
            case DE_CFG_DEFAULT_CAMERA_FOV: return m_Configs.c_DefaultCameraFOV;
            case DE_CFG_DEFAULT_CAMERA_ASPECT: return m_Configs.c_DefaultCameraAspect;
            case DE_CFG_DEFAULT_CAMERA_NEAR_PLANE: return m_Configs.c_DefaultCameraNearPlane;
            case DE_CFG_DEFAULT_CAMERA_FAR_PLANE: return m_Configs.c_DefaultCameraFarPlane;
            default: DE_ASSERT(false, "Wrong config requested (", setting, ")");
        }
        return 0;
    }
    Path Config::GetPath(U32 setting) {
        switch (setting) {
            case DE_CFG_EXECUTABLE_PATH: return m_Configs.c_ExecutablePath;
            case DE_CFG_ASSET_PATH: return m_Configs.c_AssetPath;
            case DE_CFG_LOG_PATH: return m_Configs.c_LogPath;
            case DE_CFG_MODEL_PATH: return m_Configs.c_ModelPath;
            case DE_CFG_SHADER_PATH: return m_Configs.c_ShaderPath;
            case DE_CFG_TEXTURE_PATH: return m_Configs.c_TexturePath;
            case DE_CFG_SCRIPT_PATH: return m_Configs.c_ScriptPath;
            case DE_CFG_CACHE_PATH: return m_Configs.c_CachePath;
            case DE_CFG_SCRIPT_CACHE_PATH: return m_Configs.c_ScriptCachePath;
            case DE_CFG_FONT_PATH: return m_Configs.c_FontPath;
            case DE_CFG_SCENE_PATH: return m_Configs.c_ScenePath;
            default: DE_ASSERT(false, "Wrong config requested (", setting, ")");
        }
        return Path(".");
    }
    API Config::GetRenderAPI() {
        return m_Configs.c_RenderAPI;
    }
    void Config::Initialize() {
        m_Configs.c_ExecutablePath = fs::current_path();
        std::cout << "Executable path: " << m_Configs.c_ExecutablePath.string() << std::endl;

        m_Configs.c_AssetPath       = m_Configs.c_ExecutablePath / "Assets";
        m_Configs.c_LogPath         = m_Configs.c_ExecutablePath / "Logs";
        m_Configs.c_ModelPath       = m_Configs.c_AssetPath / "Models";
        m_Configs.c_ShaderPath      = m_Configs.c_AssetPath / "Shaders";
        m_Configs.c_TexturePath     = m_Configs.c_AssetPath / "Textures";
        m_Configs.c_ScriptPath      = m_Configs.c_AssetPath / "Scripts";
        m_Configs.c_CachePath       = m_Configs.c_ExecutablePath / "Cache";
        m_Configs.c_ScriptCachePath = m_Configs.c_CachePath / "Scripts";
        m_Configs.c_FontPath        = m_Configs.c_AssetPath / "Fonts";
        m_Configs.c_ScenePath       = m_Configs.c_AssetPath / "Scenes";

        m_Configs.c_RenderAPI = API::OpenGL;
    }
    void Config::Terminate() {}

    Path RelativeToExecutable(const Path& path) {
        return fs::relative(path, Config::GetPath(DE_CFG_EXECUTABLE_PATH));
    }
}  // namespace DE