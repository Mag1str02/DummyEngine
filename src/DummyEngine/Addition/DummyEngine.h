#pragma once

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

#include "Core/ResourceManaging/ResourceManager.h"
#include "Core/ResourceManaging/AssetManager.h"

#include "Core/Scripting/ScriptInstance.h"

#include "ToolBox/Dev/Profiler.h"
#include "ToolBox/Dev/Logger.h"
#include "ToolBox/Loaders/ModelLoader.h"
#include "ToolBox/Loaders/TextureLoader.h"
#include "ToolBox/Loaders/SceneLoader.h"
#include "ToolBox/Editors/TextureEditor.h"
