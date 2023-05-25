#include "DummyEngine/Utils/ScriptIncludes.h"

using namespace DE;

class SphereSpawner : public Script {
    SCRIPT(SphereSpawner)
public:
    virtual void OnRuntimeStart(float dt) override {}
    virtual void OnRuntimeEnd(float dt) override {}

private:
    U32                 Width;
    U32                 Height;
    std::vector<Entity> spheres;
};

SCRIPT_BASE(GateController, FIELD(Width), FIELD(Height))
