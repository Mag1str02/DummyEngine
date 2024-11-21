#include "DummyEngine/Core/Objects/LightSources/LightSource.h"

namespace DummyEngine {

    std::string LightSourceTypeToString(LightSourceType type) {
        switch (type) {
            case LightSourceType::Direction: return "Direction";
            case LightSourceType::Point: return "Point";
            case LightSourceType::Spot: return "Spot";
            default: return "None";
        }
    }
    LightSourceType StringToLightSourceType(const std::string& type) {
        if (type == "Point") return LightSourceType::Point;
        if (type == "Spot") return LightSourceType::Spot;
        return LightSourceType::Direction;
    }

    U32 LightSourceTypeToId(LightSourceType type) {
        switch (type) {
            case LightSourceType::Direction: return 1;
            case LightSourceType::Point: return 2;
            case LightSourceType::Spot: return 3;
            default: return 0;
        }
    }

}  // namespace DummyEngine