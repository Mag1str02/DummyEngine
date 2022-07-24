#include "config.h"

namespace DE
{
    Config::Configs Config::m_Configs;

    uint32_t Config::Get(uint32_t setting)
    {
        switch (setting)
        {
            case DE_CFG_MAX_COMPILE_ERROR_LEN: return m_Configs.c_MaxShaderCompileErrorLen;
        }
        return 0;
    }
}  // namespace DE