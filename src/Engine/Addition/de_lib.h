#pragma once

#define STB_IMAGE_IMPLEMENTATION

#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/transform.hpp>

#include <stb_image.h>

#include "../../Config/config.h"
#include "../ToolBox/Dev/Logger/logger.h"
#include "../ToolBox/Dev/FrameTimeReader/frame_time_reader.h"
#include "../Core/ECS/BasicComponents/Transformation/transformation.h"
#include "../Core/ECS/Component/component_array.hpp"
#include "../Core/ECS/Component/component_manager.hpp"
#include "../Core/ECS/Entity/entity.hpp"
#include "../Core/ECS/Entity/entity_manager.hpp"
#include "../Core/ECS/System/system_manager.hpp"
#include "../Core/Initializer/initializer.h"
#include "../Core/Objects/Cameras/FPSCamera/fps_camera.h"
#include "../Core/Objects/LightSources/DirectionalLight/directional_light.h"
#include "../Core/Objects/LightSources/LightManager/light_manager.h"
#include "../Core/Objects/LightSources/PointLight/point_light.h"
#include "../Core/Objects/LightSources/SpotLight/spot_light.h"
#include "../Core/UserInput/InputManager/input_manager.h"
#include "../Core/Wrappings/ApplicationWindow/application_window.h"
#include "../Core/Rendering/RendererOpenGL/ShaderProgram/gl_shader_program.h"
#include "../Core/Rendering/RendererOpenGL/RenderStructs/gl_render_structs.h"
#include "../ToolBox/Loaders/ModelLoader/model_loader.h"



