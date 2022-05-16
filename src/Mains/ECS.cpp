#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>

#include "../Config/config.h"
#include "../Core/ECS/Component/component_array.hpp"
#include "../Core/ECS/Component/component_manager.hpp"
#include "../Core/ECS/Entity/entity.hpp"
#include "../Core/ECS/Entity/entity_manager.hpp"
#include "../Core/ECS/System/system.hpp"
#include "../Core/Initializer/initializer.h"
#include "../ToolBox/Dev/Logger/logger.h"

using namespace DE;

struct Transformation
{
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
};

struct Mesh
{
    unsigned int vao_id;
};

struct Health
{
    size_t health;
};

class TestSystem : public ISystem
{

public:
    TestSystem()
    {
    }
    void Update(double dt) override
    {
        auto health_array = GetComponentArray<Health>();
        std::cout << health_array->LogState() << std::endl;
    }
};

void Log()
{
    EntityManager::Get().LogState();
    ComponentManager::Get().LogState();
}

int main()
{
    Logger::Open(LOG_DIR / "ECS.txt", "ECS");
    deHint(H_MAX_ENTITY_AMOUNT, 1000);
    deInitialize();
    TestSystem test;
    Log();
    Entity a;
    Log();
    std::vector<Entity> entities;
    for (size_t i = 0; i < 5; ++i)
    {
        entities.push_back(Entity());
        auto &h = entities.back().GetComponent<Health>();
        Log();
    }

    test.Update(1);
    a.AddComponent<Health>();
    Log();

    deTerminate();
    Log();
    std::cout << "FINE!" << std::endl;
    return 0;
}
