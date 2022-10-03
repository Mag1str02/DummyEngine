#include "Addition/ScriptIncludes.h"

using namespace DE;

class CameraController : public ScriptInstance
{
public:
    CameraController() { std::cout << "CameraController created" << std::endl; }
    ~CameraController() { std::cout << "CameraController destructed" << std::endl; }
    virtual void OnAttach() override { std::cout << "Script Attached Script" << std::endl; }
    virtual void OnUpdate(float dt) override
    {
        auto& camera = GetComponent<FPSCamera>();
        camera.MoveInLocal(dt * Vec3(0.0, 0, 10.0));
    }
    virtual void OnDetach() override { std::cout << "Script Detached Script" << std::endl; }

private:
    int cnt = 0;
};

SCRIPT_INSTANCE_CREATE_FUNCTION(CameraController);