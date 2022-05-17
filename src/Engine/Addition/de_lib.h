#pragma once

#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/transform.hpp>

#include "../../Config/config.h"
#include "../ToolBox/Dev/Logger/logger.h"
#include "../Core/ECS/Component/component_array.hpp"
#include "../Core/ECS/Component/component_manager.hpp"
#include "../Core/ECS/Entity/entity.hpp"
#include "../Core/ECS/Entity/entity_manager.hpp"
#include "../Core/ECS/System/system_manager.hpp"
#include "../Core/Initializer/initializer.h"
#include "../Core/Memory/MeshManager/mesh_manager.h"
#include "../Core/Memory/ModelManager/model_manager.h"
#include "../Core/Memory/TextureManager/texture_manager.h"
#include "../Core/Memory/VAO/vao.h"
#include "../Core/Objects/Cameras/FPSCamera/fps_camera.h"
#include "../Core/Objects/LightSources/DirectionalLight/directional_light.h"
#include "../Core/Objects/LightSources/LightManager/light_manager.h"
#include "../Core/Objects/LightSources/PointLight/point_light.h"
#include "../Core/Objects/LightSources/SpotLight/spot_light.h"
#include "../Core/Objects/Model/model.h"
#include "../Core/UserInput/InputManager/input_manager.h"
#include "../Core/Wrappings/ApplicationWindow/application_window.h"
#include "../Core/Wrappings/ShaderProgram/shader_program.h"

