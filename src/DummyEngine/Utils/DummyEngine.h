#pragma once

#include "DummyEngine/Utils/Base.h"

#include "DummyEngine/Core/ECS/Entity.hpp"
#include "DummyEngine/Core/ECS/SystemManager.hpp"

#include "DummyEngine/Core/Scene/Scene.h"
#include "DummyEngine/Core/Scene/SceneHierarchy.h"
#include "DummyEngine/Core/Scene/Components.h"

#include "DummyEngine/Core/Objects/Cameras/FPSCamera.h"
#include "DummyEngine/Core/Objects/LightSources/LightSource.h"

#include "DummyEngine/Core/Rendering/Renderer/FrameBuffer.h"
#include "DummyEngine/Core/Rendering/Renderer/LocalBuffer.h"
#include "DummyEngine/Core/Rendering/Renderer/Renderer.h"

#include "DummyEngine/Core/Application/Application.h"
#include "DummyEngine/Core/Application/ImGuiPanelStack.h"

#include "DummyEngine/Core/ResourceManaging/ResourceManager.h"
#include "DummyEngine/Core/ResourceManaging/AssetManager.h"

#include "DummyEngine/Core/Scripting/ScriptManager.h"

#include "DummyEngine/ToolBox/Dev/Profiler.h"
#include "DummyEngine/ToolBox/Dev/Logger.h"
#include "DummyEngine/ToolBox/Loaders/ModelLoader.h"
#include "DummyEngine/ToolBox/Loaders/TextureLoader.h"
#include "DummyEngine/ToolBox/Loaders/SceneLoader.h"
#include "DummyEngine/ToolBox/Editors/TextureEditor.h"
