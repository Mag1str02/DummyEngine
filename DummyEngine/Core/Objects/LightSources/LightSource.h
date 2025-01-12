#pragma once

#include "DummyEngine/Utils/Types/Types.h"

namespace DummyEngine {

    enum class LightSourceType {
        Direction = 0,
        Point     = 1,
        Spot      = 2,
    };

    std::string     LightSourceTypeToString(LightSourceType type);
    LightSourceType StringToLightSourceType(const std::string& type);
    U32             LightSourceTypeToId(LightSourceType type);

    struct LightSource {
        LightSourceType Type;
        Vec3            Ambient   = Vec3(1.0f);
        Vec3            Diffuse   = Vec3(1.0f);
        Vec3            Specular  = Vec3(1.0f);
        Vec3            Direction = Vec3(0.0f, -1.0f, 0.0f);
        Vec3            Position  = Vec3(0.0f);
        Vec3            CLQ       = Vec3(1.0f, 0.007f, 0.0002f);
        float           OuterConeCosinus;
        float           InnerConeCosinus;
    };

}  // namespace DummyEngine