#include "Addition/ScriptIncludes.h"

using namespace DE;

class CameraController : public ScriptInstance
{
public:
    CameraController() : coord3_ref(coord3)
    {
        ADD_FIELD(speed);
        ADD_FIELD(field_float);
        ADD_FIELD(check);
        ADD_FIELD(i32);
        ADD_FIELD(i64);
        ADD_FIELD(ui32);
        ADD_FIELD(ui64);
        ADD_FIELD(coord2);
        ADD_FIELD(coord3);
        ADD_FIELD(coord4);
    }

    virtual void OnAttach() override {}
    virtual void OnUpdate(float dt) override
    {
        auto& camera = GetComponent<FPSCamera>();
        camera.MoveInLocal((float)speed * dt * Vec3(0.0, 0, -10.0));
    }
    virtual void OnDetach() override { std::cout << "Script Detached Script" << std::endl; }

private:
    double      speed       = 1.0;
    bool        check       = true;
    float       field_float = 1.0;
    int32_t     i32         = 0;
    int64_t     i64         = 0;
    uint32_t    ui32        = 0;
    uint64_t    ui64        = 0;
    std::string name        = "hello";
    Vec2        coord2      = Vec2(1.0, 0.0);
    Vec3        coord3      = Vec3(1.0, 1.0, 0.0);
    Vec4        coord4      = Vec4(1.0, 1.0, 1.0, 0.0);
    Vec3&       coord3_ref;
};

SCRIPT_INSTANCE_CREATE_FUNCTION(CameraController);