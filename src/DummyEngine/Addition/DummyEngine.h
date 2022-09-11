#pragma once

#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

#include <stb_image.h>
#include <stb_image_write.h>

#include <ImGui/imgui.h>
#include <ImGui/backends/imgui_impl_glfw.h>
#include <ImGui/backends/imgui_impl_opengl3.h>
#include <ImGui/misc/cpp/imgui_stdlib.h>

#include "Addition/Base.h"

#include "Core/ECS/Entity.hpp"
#include "Core/ECS/SystemManager.hpp"

#include "Core/Scene/Scene.h"
#include "Core/Scene/Components.h"

#include "Core/Objects/Cameras/FPSCamera.h"
#include "Core/Objects/LightSources/LightSource.h"

#include "Core/Rendering/Renderer/FrameBuffer.h"
#include "Core/Rendering/Renderer/LocalBuffer.h"
#include "Core/Rendering/Renderer/Renderer.h"

#include "Core/Application/Application.h"
#include "Core/Application/ImGuiPanelStack.h"

#include "ToolBox/Dev/Profiler.h"
#include "ToolBox/Dev/Logger.h"
#include "ToolBox/Loaders/ModelLoader.h"
#include "ToolBox/Loaders/TextureLoader.h"
#include "ToolBox/Loaders/SceneLoader.h"
#include "ToolBox/Editors/TextureEditor.h"
