#pragma once

#define STB_IMAGE_IMPLEMENTATION

#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

#include <stb_image.h>

#include "DummyEngine/Addition/Base.h"
#include "DummyEngine/Config/Config.h"

#include "DummyEngine/Core/ECS/Entity.hpp"
#include "DummyEngine/Core/ECS/SystemManager.hpp"
#include "DummyEngine/Core/ECS/Transformation.h"

#include "DummyEngine/Core/Initializer/Initializer.h"
#include "DummyEngine/Core/Input/Input.h"
#include "DummyEngine/Core/Objects/Cameras/FPSCamera.h"
#include "DummyEngine/Core/Objects/LightSources/LightManager.h"

#include "DummyEngine/Core/Rendering/Renderer/Renderer.h"

#include "DummyEngine/Core/Wrappings/Window.h"
#include "DummyEngine/Core/Wrappings/Application.h"
#include "DummyEngine/ToolBox/Dev/FrameTimeReader.h"
#include "DummyEngine/ToolBox/Dev/Logger.h"
#include "DummyEngine/ToolBox/Loaders/ModelLoader.h"
