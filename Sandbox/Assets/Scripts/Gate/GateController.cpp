#include "DummyEngine/Utils/ScriptIncludes.h"

using namespace DE;

class GateController : public Script {
    SCRIPT(GateController)
public:
    virtual void OnCreate() override {}
    virtual void OnUpdate(float dt) override {
        m_Open            = false;
        Entity controller = GetEntityByName(ControllerName);
        if (controller.Valid()) {
            const auto& controller_pos = controller.Get<TransformComponent>().translation;
            const auto& my_pos         = Get<TransformComponent>().translation;
            float       dist           = glm::length(Vec2(my_pos.x - controller_pos.x, my_pos.z - controller_pos.z));
            m_Open                     = (dist < Distance);
        }
        Move();
    }
    virtual void OnDestroy() override {}

private:
    void Move() {
        if (m_Open) {
            if (current_height + Speed < OpenHeight) {
                current_height += Speed;
            } else {
                current_height = OpenHeight;
            }
        } else {
            if (current_height - Speed > CloseHeight) {
                current_height -= Speed;
            } else {
                current_height = CloseHeight;
            }
        }
        auto& my_pos         = Get<TransformComponent>();
        my_pos.translation.y = current_height;
        my_pos.scale.y       = (OpenHeight - current_height) / (OpenHeight - CloseHeight) * 3 + 1;
    }

    bool        m_Open         = false;
    float       Speed          = 1.0;
    float       Distance       = 1.0;
    float       current_height = 0.0;
    float       OpenHeight     = 0;
    float       CloseHeight    = 0;
    std::string ControllerName;
};

SCRIPT_BASE(GateController, FIELD(Speed), FIELD(Distance), FIELD(ControllerName), FIELD(OpenHeight), FIELD(CloseHeight))
