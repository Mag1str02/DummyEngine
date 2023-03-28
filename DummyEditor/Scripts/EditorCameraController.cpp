#include "DummyEditor/DummyEngineInclude.h"

using namespace DE;

class EditorCameraController : public Script {
    SCRIPT(EditorCameraController)
public:
    virtual void OnCreate() override {}
    virtual void OnUpdate(float dt) override {
        if (active) {
            auto& camera      = Get<FPSCamera>();
            float speed       = 15;
            float sensitivity = 0.07;

            camera.RotateY(Input::CursorXOffset() * sensitivity);
            camera.RotateX(Input::CursorYOffset() * sensitivity / 16 * 9);

            if (Input::KeyDown(Key::LeftShift)) {
                speed = 100.0f;
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
    }
    virtual void OnDestroy() override {}

private:
    bool active = false;
};

SCRIPT_BASE(EditorCameraController, FIELD(active))
