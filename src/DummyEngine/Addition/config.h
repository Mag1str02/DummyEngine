#pragma once

#include "Base.h"

namespace DE
{

#define DE_CFG_MAX_COMPILE_ERROR_LEN 1

    class Config
    {
    public:
        static uint32_t Get(uint32_t setting);

    private:
        struct Configs
        {
            uint32_t c_MaxShaderCompileErrorLen = 512;
        };
        static Configs m_Configs;
    };

#define DE_ENABLE_FRAME_TIME_READER 1
}  // namespace DE