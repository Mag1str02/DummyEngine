#include "Addition/ScriptIncludes.h"

using namespace DE;

class EditorCameraController : public ScriptInstance
{
public:
    EditorCameraController() {
        ADD_FIELD(active);
    }

    virtual void OnAttach() override {}
    virtual void OnUpdate(float dt) override
    {
        if (active)
        {
            auto& camera      = GetComponent<FPSCamera>();
            float speed       = 15;
            float sensitivity = 0.07;

            camera.RotateY(Input::CursorXOffset() * sensitivity);
            camera.RotateX(Input::CursorYOffset() * sensitivity / 16 * 9);

            if (Input::KeyDown(KEY_LEFT_SHIFT))
            {
                speed = 100.0f;
            }
            if (Input::KeyDown(KEY_S))
            {
                camera.MoveInLocal(Vec3(0.0f, 0.0f, -1.0f) * speed * dt);
            }
            if (Input::KeyDown(KEY_W))
            {
                camera.MoveInLocal(Vec3(0.0f, 0.0f, 1.0f) * speed * dt);
            }
            if (Input::KeyDown(KEY_D))
            {
                camera.MoveInLocal(Vec3(1.0f, 0.0f, 0.0f) * speed * dt);
            }
            if (Input::KeyDown(KEY_A))
            {
                camera.MoveInLocal(Vec3(-1.0f, 0.0f, 0.0f) * speed * dt);
            }
            if (Input::KeyDown(KEY_SPACE))
            {
                camera.MoveInWorld(Vec3(0.0f, 1.0f, 0.0f) * speed * dt);
            }
            if (Input::KeyDown(KEY_C))
            {
                camera.MoveInWorld(Vec3(0.0f, -1.0f, 0.0f) * speed * dt);
            }
        }
    }
    virtual void OnDetach() override {}

private:
    bool active = false;
};

SCRIPT_INSTANCE_CREATE_FUNCTION(EditorCameraController);