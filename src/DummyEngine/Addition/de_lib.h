#pragma once

#define STB_IMAGE_IMPLEMENTATION

#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

#include <stb_image.h>

#include "DummyEngine/Addition/base.h"
#include "DummyEngine/Config/config.h"

#include "DummyEngine/Core/ECS/entity.hpp"
#include "DummyEngine/Core/ECS/system_manager.hpp"
#include "DummyEngine/Core/ECS/transformation.h"

#include "DummyEngine/Core/Initializer/initializer.h"
#include "DummyEngine/Core/Input/input.h"
#include "DummyEngine/Core/Objects/Cameras/fps_camera.h"
#include "DummyEngine/Core/Objects/LightSources/directional_light.h"
#include "DummyEngine/Core/Objects/LightSources/light_manager.h"
#include "DummyEngine/Core/Objects/LightSources/point_light.h"
#include "DummyEngine/Core/Objects/LightSources/spot_light.h"

#include "DummyEngine/Core/Rendering/Renderer/renderer.h"

#include "DummyEngine/Core/Wrappings/window.h"
#include "DummyEngine/Core/Wrappings/application.h"
#include "DummyEngine/ToolBox/Dev/frame_time_reader.h"
#include "DummyEngine/ToolBox/Dev/logger.h"
#include "DummyEngine/ToolBox/Loaders/model_loader.h"
