#pragma once

#include "Base.h"

namespace DE
{

#define DE_CFG_MAX_COMPILE_ERROR_LEN 1
#define DE_CFG_MAX_INPUT_FRAME_AMOUNT 2
#define DE_CFG_MAX_MESSAGE_TYPE_LENGTH 3

#define DE_CFG_DEFAULT_CAMERA_FOV 4
#define DE_CFG_DEFAULT_CAMERA_ASPECT 5
#define DE_CFG_DEFAULT_CAMERA_NEAR_PLANE 6
#define DE_CFG_DEFAULT_CAMERA_FAR_PLANE 7

    class Config
    {
    public:
        static uint32_t GetI(uint32_t setting);
        static float GetF(uint32_t setting);

    private:
        struct Configs
        {
            uint32_t c_MaxShaderCompileErrorLen = 512;
            uint32_t c_MaxInputFrameAmount = 100;
            uint32_t c_MaxMessageTypeLength = 7;

            float c_DefaultCameraFOV = 60.0f;
            float c_DefaultCameraAspect = 16.0f / 9.0f;
            float c_DefaultCameraNearPlane = 0.0000001f;
            float c_DefaultCameraFarPlane = 2000.0f;
        };
        static Configs m_Configs;
    };
}  // namespace DE