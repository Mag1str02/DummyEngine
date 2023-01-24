#include "DummyEngine/Utils/ScriptIncludes.h"

using namespace DE;

class SquareMovement;

class SquareMovement : public ScriptInstance
{
public:
    SquareMovement() : current_time(0.0f), cycle_time(1.0f), height(0.0), left_back({0.0, 0.0}), right_front({0.0, 0.0})
    {
        ADD_FIELD(height);
        ADD_FIELD(offset);
        ADD_FIELD(left_back);
        ADD_FIELD(right_front);
        ADD_FIELD(cycle_time);
    }

    virtual void OnCreate() override {}
    virtual void OnUpdate(float dt) override
    {
        current_time += dt;
        float persent  = ((current_time + offset) - cycle_time * (int)((current_time + offset) / cycle_time)) / cycle_time;
        auto& position = GetComponent<TransformComponent>().translation;
        if (persent <= 0.25f)
        {
            position = Vec3(left_back.x, height, left_back.y) + Smouth(4.0f * (persent - 0.0f)) * Vec3(right_front.x - left_back.x, 0, 0);
        }
        else if (persent <= 0.5f)
        {
            position = Vec3(right_front.x, height, left_back.y) + Smouth(4.0f * (persent - 0.25f)) * Vec3(0, 0, right_front.y - left_back.y);
        }
        else if (persent <= 0.75f)
        {
            position = Vec3(right_front.x, height, right_front.y) + Smouth(4.0f * (persent - 0.5f)) * Vec3(left_back.x - right_front.x, 0, 0);
        }
        else if (persent <= 1.0f)
        {
            position = Vec3(left_back.x, height, right_front.y) + Smouth(4.0f * (persent - 0.75f)) * Vec3(0, 0, left_back.y - right_front.y);
        }
    }
    virtual void OnDestroy() override {}

private:
    float Smouth(float delta) const { return (std::sin((delta * 2 - 1) * 3.1415926 / 2) + 1) / 2; }

    float cycle_time;
    float current_time;
    float height;
    float offset;
    Vec2  left_back;
    Vec2  right_front;
};

SCRIPT_BASE(SquareMovement)
SCRIPT_INSTANCE_CREATE_FUNCTION(SquareMovement)