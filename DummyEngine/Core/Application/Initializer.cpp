#include "Initializer.h"

#include "DummyEngine/Core/Application/Application.h"
#include "DummyEngine/Core/Application/Config.h"
#include "DummyEngine/Core/Application/GLFW.h"
#include "DummyEngine/Core/Application/Input.h"
#include "DummyEngine/Core/Console/Console.hpp"
#include "DummyEngine/Core/Rendering/Renderer/Renderer.h"
#include "DummyEngine/Core/ResourceManaging/AssetManager.h"
#include "DummyEngine/Core/ResourceManaging/ResourceManager.h"
#include "DummyEngine/Core/Scripting/ScriptEngine.h"
#include "DummyEngine/Utils/Debug/Profiler.h"

#include <imgui.h>

namespace DummyEngine {

    void Initializer::Initialize() {
        PreInitialize();
        EngineInitialize();
    }
    void Initializer::Terminate() {
        EngineTerminate();
        PostTerminate();
    }

    void Initializer::PreInitialize() {
        Config::Initialize();
        Console::Initialize();
        Logger::Initialize();

        ImGui::g_ImGuiFailAssert                                    = FailAssert;
        ImGui::g_ExternalSettings.DragAndDropTooltipAlphaMultiplyer = 1.0;

        LOG_INFO("Engine PreInitialize complete");
    }
    void Initializer::EngineInitialize() {
        DE_PROFILER_BEGIN_FRAME();

        LOG_INFO("Initializing Engine");
        AssetManager::Initialize();
        ResourceManager::Initialize();
        ScriptEngine::Initialize();
        Input::Initialize();
        Application::Initialize();
        Renderer::Initialize();

        GLFW::StartEventProcessing();
    }

    void Initializer::EngineTerminate() {
        LOG_INFO("Terminating Engine");

        GLFW::StopEventProcessing();

        Renderer::Terminate();
        Application::Terminate();
        Input::Terminate();
        ScriptEngine::Terminate();
        ResourceManager::Terminate();
        AssetManager::Terminate();
    }
    void Initializer::PostTerminate() {
        LOG_INFO("Egnine PostTerminating...");

        Logger::Terminate();
        Config::Terminate();
    }
}  // namespace DummyEngine
