#include "DummyEngine/Addition/Config.h"
#include "DummyEngine/Addition/Assert.h"

namespace DE
{
    Config::Configs Config::m_Configs;

    uint32_t Config::GetI(uint32_t setting)
    {
        switch (setting)
        {
            case DE_CFG_MAX_COMPILE_ERROR_LEN: return m_Configs.c_MaxShaderCompileErrorLen;
            case DE_CFG_MAX_INPUT_FRAME_AMOUNT: return m_Configs.c_MaxInputFrameAmount;
            case DE_CFG_MAX_MESSAGE_TYPE_LENGTH: return m_Configs.c_MaxMessageTypeLength;
            default: DE_ASSERT(false, "Wrong config requested.");
        }
        return 0;
    }
    float Config::GetF(uint32_t setting)
    {
        switch (setting)
        {
            case DE_CFG_DEFAULT_CAMERA_FOV: return m_Configs.c_DefaultCameraFOV;
            case DE_CFG_DEFAULT_CAMERA_ASPECT: return m_Configs.c_DefaultCameraAspect;
            case DE_CFG_DEFAULT_CAMERA_NEAR_PLANE: return m_Configs.c_DefaultCameraNearPlane;
            case DE_CFG_DEFAULT_CAMERA_FAR_PLANE: return m_Configs.c_DefaultCameraFarPlane;
            default: DE_ASSERT(false, "Wrong config requested.");
        }
        return 0;
    }
    Path Config::GetPath(uint32_t setting)
    {
        switch (setting)
        {
            case DE_CFG_EXECUTABLE_PATH : return m_Configs.c_ExecutablePath;
            default: DE_ASSERT(false, "Wrong config requested.");
        }
        return Path(".");
    }
    void Config::Init()
    {
        m_Configs.c_ExecutablePath = fs::current_path();
    }
}  // namespace DE