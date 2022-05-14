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
#include "../Core/Initializer/initializer.h"
#include "../ToolBox/Dev/Logger/logger.h"

using namespace DE;

struct Transformation {
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
};

struct Mesh {
    unsigned int vao_id;
};

struct Health {
    size_t health;
};

int main() {
    Logger::Open(LOG_DIR / "ECS.txt", "ECS");
    deInitialize();
    std::vector<Entity> entities(10);
    entities[0].Create();
    deTerminate();
    std::cout << "FINE!" << std::endl;
    return 0;
}
