#include "DummyEngine/DummyEngine.h"

using namespace DummyEngine;

class SquareMovement : public Script {
    SCRIPT(SquareMovement)
public:
    virtual void OnUpdate(float dt) override {
        current_time_ += dt;
        float persent  = ((current_time_ + offset_) - cycle_time_ * (int)((current_time_ + offset_) / cycle_time_)) / cycle_time_;
        auto& position = Get<TransformComponent>().Translation;
        if (persent <= 0.25f) {
            position = Vec3(left_back_.x, height_, left_back_.y) + Smouth(4.0f * (persent - 0.0f)) * Vec3(right_front_.x - left_back_.x, 0, 0);
        } else if (persent <= 0.5f) {
            position = Vec3(right_front_.x, height_, left_back_.y) + Smouth(4.0f * (persent - 0.25f)) * Vec3(0, 0, right_front_.y - left_back_.y);
        } else if (persent <= 0.75f) {
            position = Vec3(right_front_.x, height_, right_front_.y) + Smouth(4.0f * (persent - 0.5f)) * Vec3(left_back_.x - right_front_.x, 0, 0);
        } else if (persent <= 1.0f) {
            position = Vec3(left_back_.x, height_, right_front_.y) + Smouth(4.0f * (persent - 0.75f)) * Vec3(0, 0, left_back_.y - right_front_.y);
        }
    }

private:
    float Smouth(float delta) const { return (std::sin((delta * 2 - 1) * 3.1415926 / 2) + 1) / 2; }

    float cycle_time_   = 1;
    float current_time_ = 0;
    float height_       = 10;
    float offset_       = 0;
    Vec2  left_back_    = {10, 10};
    Vec2  right_front_  = {0.0, 0.0};
};

SCRIPT_BASE(SquareMovement, FIELD(cycle_time_), FIELD(height_), FIELD(offset_), FIELD(left_back_), FIELD(right_front_))
