#include "DummyEngine/Utils/ScriptIncludes.h"

using namespace DE;

class SquareMovement : public Script {
    SCRIPT(SquareMovement)
public:
    virtual void OnUpdate(float dt) override {
        current_time += dt;
        float persent  = ((current_time + offset) - cycle_time * (int)((current_time + offset) / cycle_time)) / cycle_time;
        auto& position = Get<TransformComponent>().translation;
        if (persent <= 0.25f) {
            position = Vec3(left_back.x, height, left_back.y) + Smouth(4.0f * (persent - 0.0f)) * Vec3(right_front.x - left_back.x, 0, 0);
        } else if (persent <= 0.5f) {
            position = Vec3(right_front.x, height, left_back.y) + Smouth(4.0f * (persent - 0.25f)) * Vec3(0, 0, right_front.y - left_back.y);
        } else if (persent <= 0.75f) {
            position = Vec3(right_front.x, height, right_front.y) + Smouth(4.0f * (persent - 0.5f)) * Vec3(left_back.x - right_front.x, 0, 0);
        } else if (persent <= 1.0f) {
            position = Vec3(left_back.x, height, right_front.y) + Smouth(4.0f * (persent - 0.75f)) * Vec3(0, 0, left_back.y - right_front.y);
        }
    }

private:
    float Smouth(float delta) const { return (std::sin((delta * 2 - 1) * 3.1415926 / 2) + 1) / 2; }

    float cycle_time   = 1;
    float current_time = 0;
    float height       = 10;
    float offset       = 0;
    Vec2  left_back    = {10, 10};
    Vec2  right_front  = {0.0, 0.0};
};

SCRIPT_BASE(SquareMovement, FIELD(cycle_time), FIELD(height), FIELD(offset), FIELD(left_back), FIELD(right_front))
