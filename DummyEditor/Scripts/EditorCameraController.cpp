#include "DummyEngine/DummyEngine.h"

using namespace DummyEngine;

class EditorCameraController : public Script {
    SCRIPT(EditorCameraController)
public:
    virtual void OnUpdate(float dt) override {
        auto& camera = Get<FPSCamera>();
        if (active_) {
            float speed       = 15;
            float sensitivity = 0.07;

            camera.RotateY(Input::CursorXOffset() * sensitivity);
            camera.RotateX(Input::CursorYOffset() * sensitivity / 16 * 9);

            if (Input::KeyDown(Key::LeftShift)) {
                speed *= 10.0f;
            }
            if (Input::KeyDown(Key::LeftControl)) {
                speed /= 10.0f;
            }
            if (Input::KeyDown(Key::S)) {
                camera.MoveInLocal(Vec3(0.0f, 0.0f, -1.0f) * speed * dt);
            }
            if (Input::KeyDown(Key::W)) {
                camera.MoveInLocal(Vec3(0.0f, 0.0f, 1.0f) * speed * dt);
            }
            if (Input::KeyDown(Key::D)) {
                camera.MoveInLocal(Vec3(1.0f, 0.0f, 0.0f) * speed * dt);
            }
            if (Input::KeyDown(Key::A)) {
                camera.MoveInLocal(Vec3(-1.0f, 0.0f, 0.0f) * speed * dt);
            }
            if (Input::KeyDown(Key::Space)) {
                camera.MoveInWorld(Vec3(0.0f, 1.0f, 0.0f) * speed * dt);
            }
            if (Input::KeyDown(Key::C)) {
                camera.MoveInWorld(Vec3(0.0f, -1.0f, 0.0f) * speed * dt);
            }
        }
        Get<TransformComponent>().Translation = camera.GetPos();
    }

private:
    bool active_ = false;
};

SCRIPT_BASE(EditorCameraController, FIELD("Active", active_))
