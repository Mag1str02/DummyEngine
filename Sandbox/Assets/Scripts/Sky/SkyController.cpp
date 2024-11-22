#include "DummyEngine/DummyEngine.h"

using namespace DummyEngine;

class SkyContoller : public Script {
    SCRIPT(SkyContoller)
public:
    virtual void OnRuntimeStop() override {
        auto& pos = Get<TransformComponent>().Rotation;
        pos       = {0.0, 0.0, 0.0};
    }
    virtual void OnUpdate(float dt) override {
        DE_PROFILE_SCOPE("SkyController");
        current_time_ += dt;
        auto& pos = Get<TransformComponent>().Rotation;
        pos.x += dt * rotation_speed_.x;
        pos.y += dt * rotation_speed_.y;
        pos.z += dt * rotation_speed_.z;
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
    Vec3  rotation_speed_ = {0.1, 0.1, 0.1};
    float current_time_   = 0;
};

SCRIPT_BASE(SkyContoller, FIELD("RotationSpeed", rotation_speed_))
