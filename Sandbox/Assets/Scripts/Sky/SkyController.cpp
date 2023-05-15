#include "DummyEngine/Utils/ScriptIncludes.h"

using namespace DE;

class SkyContoller : public Script {
    SCRIPT(SkyContoller)
public:
    virtual void OnRuntimeStop() {
        auto& pos = Get<TransformComponent>().rotation;
        pos       = {0.0, 0.0, 0.0};
    }
    virtual void OnUpdate(float dt) override {
        DE_PROFILE_SCOPE("SkyController");
        current_time += dt;
        auto& pos = Get<TransformComponent>().rotation;
        pos.x += dt * RotationSpeed.x;
        pos.y += dt * RotationSpeed.y;
        pos.z += dt * RotationSpeed.z;
        ClampRoundValue(pos, 0, 360);
    }

private:
    void ClampRoundValue(Vec3& vec, float min, float max) {
        if (vec.x < min) vec.x = max;
        if (vec.x > max) vec.x = min;
        if (vec.y < min) vec.y = max;
        if (vec.y > max) vec.y = min;
        if (vec.z < min) vec.z = max;
        if (vec.z > max) vec.z = min;
    }
    Vec3  RotationSpeed = {0.1, 0.1, 0.1};
    float current_time  = 0;
};

SCRIPT_BASE(SkyContoller, FIELD(RotationSpeed))
