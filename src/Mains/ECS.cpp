#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>

#include "../Config/config.h"
#include "../Core/Addition/de_lib.h"
#include "../ToolBox/Dev/Logger/logger.h"

using namespace DE;

struct Transformation {
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
};

struct Health {
    double health;
};
struct Healing {
    double heal_per_sec;
};

class HealingSystem : public System {

public:
    HealingSystem() {
    }
    void Update(double dt) override {
        auto& health_array = GetComponentArray<Health>();
        auto& healing_array = GetComponentArray<Healing>();
        for (auto it : health_array) {
            if (healing_array.HasComponent(it.first)) {
                it.second.health += dt * healing_array[it.first].heal_per_sec;
            }
        }
    }
};

void Test1() {
    deHint(H_MAX_ENTITY_AMOUNT, 1000);
    deInitialize();
    Entity::Log();

    Entity* a = new Entity();
    Entity::Log();

    a->AddComponent<Health>();
    Entity::Log();

    std::vector<Entity> entities;
    for (size_t i = 0; i < 5; ++i) {
        entities.push_back(Entity());
        auto& h = entities.back().GetComponent<Health>();
        Entity::Log();
    }

    Entity b = *a;
    a->RemoveComponent<Health>();
    Entity::Log();

    delete a;
    entities[2].AddComponent<Transformation>();
    Entity::Log();

    deTerminate();
    Entity::Log();
}
void Test2() {
    deInitialize();
    SystemManager::RegisterSystem<HealingSystem>();
    SystemManager::CalculateOrder();
    Entity::Log();

    std::vector<Entity> entities(5);
    std::string log_healing;
    std::string log_health;

    log_healing = "Healing: ";
    log_health = "Health: ";
    for (size_t i = 0; i < 5; ++i) {
        entities[i].AddComponent<Health>(Health(i * 10));
        entities[i].AddComponent<Healing>(Healing((5 - i) * 5));
        log_healing.append(std::to_string(entities[i].GetComponent<Healing>().heal_per_sec) + " ");
        log_health.append(std::to_string(entities[i].GetComponent<Health>().health) + " ");
    }
    entities[4].RemoveComponent<Healing>();
    Logger::Info("ECS", "Main", log_health);
    Logger::Info("ECS", "Main", log_healing);
    Entity::Log();

    SystemManager::Update(3);

    log_healing = "Healing: ";
    log_health = "Health: ";
    for (size_t i = 0; i < 5; ++i) {
        log_health.append(std::to_string(entities[i].GetComponent<Health>().health) + " ");
        if (i != 4) {
            log_healing.append(std::to_string(entities[i].GetComponent<Healing>().heal_per_sec) + " ");
        }
    }
    Logger::Info("ECS", "Main", log_health);
    Logger::Info("ECS", "Main", log_healing);
    Entity::Log();

    deTerminate();
    Entity::Log();
}

int main() {
    Logger::Open(LOG_DIR / "ECS.txt", "ECS");
    Logger::Stage("ECS", "Main", "TEST 1");
    Test1();
    Logger::Stage("ECS", "Main", "TEST 2");
    Test2();
    std::cout << "FINE!" << std::endl;
    return 0;
}
