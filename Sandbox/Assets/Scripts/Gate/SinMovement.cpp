#include "DummyEngine/DummyEngine.h"

using namespace DummyEngine;

class SinMovement : public Script {
    SCRIPT(SinMovement)
public:
    virtual void OnUpdate(float dt) override {
        DE_PROFILE_SCOPE("SinMovement");
        current_time_ += dt * speed_;
        auto& pos = Get<TransformComponent>().Translation;
        float d   = (sin(current_time_) + 1) / 2;
        pos       = d * pos_a_ + (1 - d) * pos_b_;
        // pos = d * PosA + (1 - d) * PosB + Vec3(0, 20, 0)* float(cos(current_time)) ;
    }

private:
    Vec3  pos_a_        = Vec3(0, 0, 0);
    Vec3  pos_b_        = Vec3(0, 0, 0);
    float speed_        = 1;
    float current_time_ = 0;
};

SCRIPT_BASE(SinMovement, FIELD("PosA", pos_a_), FIELD("PosB", pos_b_), FIELD("Speed", speed_))
