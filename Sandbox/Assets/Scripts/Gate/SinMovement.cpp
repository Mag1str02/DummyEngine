#include "DummyEngine/Utils/ScriptIncludes.h"

using namespace DE;

class SinMovement : public Script {
    SCRIPT(SinMovement)
public:
    virtual void OnCreate() override {}
    virtual void OnUpdate(float dt) override {
        current_time += dt * Speed;
        auto& pos = Get<TransformComponent>().translation;
        float d   = (sin(current_time) + 1) / 2;
        pos       = d * PosA + (1 - d) * PosB;
    }
    virtual void OnDestroy() override {}

private:
    Vec3  PosA         = Vec3(0, 0, 0);
    Vec3  PosB         = Vec3(0, 0, 0);
    float Speed        = 1;
    float current_time = 0;
};

SCRIPT_BASE(SinMovement, FIELD(PosA), FIELD(PosB), FIELD(Speed))
