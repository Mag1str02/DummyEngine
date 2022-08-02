#pragma once

#define STB_IMAGE_IMPLEMENTATION

#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

#include <stb_image.h>

#include "Addition/Base.h"

#include "Core/ECS/Entity.hpp"
#include "Core/ECS/SystemManager.hpp"

#include "Core/Scene/Scene.h"
#include "Core/Scene/Components.h"

#include "Core/Initializer/Initializer.h"
#include "Core/Input/Input.h"
#include "Core/Objects/Cameras/FPSCamera.h"
#include "Core/Objects/LightSources/LightManager.h"

#include "Core/Rendering/Renderer/Renderer.h"

#include "Core/Wrappings/Window.h"
#include "Core/Wrappings/Application.h"
#include "ToolBox/Dev/FrameTimeReader.h"
#include "ToolBox/Dev/Logger.h"
#include "ToolBox/Loaders/ModelLoader.h"
