#include "Core/Objects/LightSources/LightSource.h"

namespace DE
{

    std::string LightSourceTypeToString(LightSourceType type)
    {
        switch (type)
        {
            case LightSourceType::Direction: return "Direction";
            case LightSourceType::Point: return "Point";
            case LightSourceType::Spot: return "Spot";
            default: return "None";
        }
    }
    LightSourceType StringToLightSourceType(const std::string& type)
    {
        if (type == "Direction") return LightSourceType::Direction;
        if (type == "Point") return LightSourceType::Point;
        if (type == "Spot") return LightSourceType::Spot;
        return LightSourceType::None;
    }
    
    uint32_t LightSourceTypeToId(LightSourceType type)
    {
        switch (type)
        {
            case LightSourceType::Direction: return 1;
            case LightSourceType::Point: return 2;
            case LightSourceType::Spot: return 3;
            default: return 0;
        }
    }
}  // namespace DE