#pragma once

#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/transform.hpp>

#include "../../Config/config.h"
#include "../../ToolBox/Dev/Logger/logger.h"
#include "../ECS/Component/component_array.hpp"
#include "../ECS/Component/component_manager.hpp"
#include "../ECS/Entity/entity.hpp"
#include "../ECS/Entity/entity_manager.hpp"
#include "../ECS/System/system_manager.hpp"
#include "../Initializer/initializer.h"
#include "../Memory/MeshManager/mesh_manager.h"
#include "../Memory/ModelManager/model_manager.h"
#include "../Memory/TextureManager/texture_manager.h"
#include "../Memory/VAO/vao.h"
#include "../Objects/Cameras/FPSCamera/fps_camera.h"
#include "../Objects/LightSources/DirectionalLight/directional_light.h"
#include "../Objects/LightSources/LightManager/light_manager.h"
#include "../Objects/LightSources/PointLight/point_light.h"
#include "../Objects/LightSources/SpotLight/spot_light.h"
#include "../Objects/Model/model.h"
#include "../UserInput/InputManager/input_manager.h"
#include "../Wrappings/ApplicationWindow/application_window.h"
#include "../Wrappings/ShaderProgram/shader_program.h"

