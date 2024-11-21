#include "DummyEngine/DummyEngine.h"

using namespace DummyEngine;

class GateController : public Script {
    SCRIPT(GateController)
public:
    virtual void OnUpdate(float) override {
        DE_PROFILE_SCOPE("GateContoller");
        open_ = false;
        if (controller_.Valid()) {
            if (controller_.Has<TransformComponent>()) {
                const auto& controller_pos = controller_.Get<TransformComponent>().Translation;
                const auto& my_pos         = Get<TransformComponent>().Translation;
                float       dist           = glm::length(Vec2(my_pos.x - controller_pos.x, my_pos.z - controller_pos.z));
                open_                      = (dist < distance_);
            }
        }
        Move();
    }

private:
    void Move() {
        if (open_) {
            if (current_height_ + speed_ < open_height_) {
                current_height_ += speed_;
            } else {
                current_height_ = open_height_;
            }
        } else {
            if (current_height_ - speed_ > close_height_) {
                current_height_ -= speed_;
            } else {
                current_height_ = close_height_;
            }
        }
        auto& my_pos         = Get<TransformComponent>();
        my_pos.Translation.y = current_height_;
        my_pos.Scale.y       = (open_height_ - current_height_) / (open_height_ - close_height_) * 3 + 1;
    }

    bool   open_           = false;
    float  speed_          = 1.0;
    float  distance_       = 1.0;
    float  current_height_ = 0.0;
    float  open_height_    = 0;
    float  close_height_   = 0;
    Entity controller_;
};

SCRIPT_BASE(GateController, FIELD(speed_), FIELD(distance_), FIELD(open_height_), FIELD(close_height_), FIELD(controller_))
