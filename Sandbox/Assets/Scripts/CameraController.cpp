#include "Addition/ScriptIncludes.h"

using namespace DE;

class CameraController : public ScriptInstance
{
public:
    CameraController() { ADD_FIELD(speed); }

    virtual void OnAttach() override {}
    virtual void OnUpdate(float dt) override
    {
        auto& camera = GetComponent<FPSCamera>();
        camera.MoveInLocal(speed * dt * Vec3(0.0, 0, -10.0));
    }
    virtual void OnDetach() override { std::cout << "Script Detached Script" << std::endl; }

private:
    float speed = 1.0;
};

SCRIPT_INSTANCE_CREATE_FUNCTION(CameraController);