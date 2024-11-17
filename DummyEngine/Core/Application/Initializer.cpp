#include "DummyEngine/Core/Application/Initializer.h"

// clang-format off
#include <imgui.h>
// clang-format on

#include "DummyEngine/Core/Application/Application.h"
#include "DummyEngine/Core/Application/Config.h"
#include "DummyEngine/Core/Console/Console.hpp"
#include "DummyEngine/Core/Rendering/Renderer/Renderer.h"
#include "DummyEngine/Core/ResourceManaging/AssetManager.h"
#include "DummyEngine/Core/ResourceManaging/ResourceManager.h"
#include "DummyEngine/Core/Scripting/ScriptEngine.h"
#include "DummyEngine/Utils/Base.h"

namespace DE {
    void error_callback(int error, const char* description) {
        fprintf(stderr, "Error: %s\n", description);
        fflush(stderr);
    }
    void Initializer::Initialize() {
        PreInitialize();
        DepInitialize();
        EngineInitialize();
    }
    void Initializer::Terminate() {
        EngineTerminate();
        DepTerminate();
        PostTerminate();
    }

    void Initializer::PreInitialize() {
        Profiler::Initialize();
        Config::Initialize();
        Console::Initialize();
        Logger::Initialize();
        LOG_INFO("Initializer", "Logger and config initialized");
    }
    void Initializer::DepInitialize() {
        LOG_INFO("Initializer", "Initializing dependencies");
        //* Init GLFW
        {
            if (!glfwInit()) {
                DE_ASSERT(false, "Failed to initialize GLFW");
            }
            glfwSetErrorCallback(error_callback);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            ImGui::g_ImGuiFailAssert                                    = FailAssert;
            ImGui::g_ExternalSettings.DragAndDropTooltipAlphaMultiplyer = 1.0;
            LOG_INFO("Initializer", "Initialized GLFW");
        }
    }
    void Initializer::EngineInitialize() {
        DE_PROFILER_BEGIN_FRAME();

        LOG_INFO("Initializer", "Initializing Engine");
        AssetManager::Initialize();
        ResourceManager::Initialize();
        ScriptEngine::Initialize();
        Input::Initialize();
        Application::Initialize();
        Renderer::Initialize();
    }

    void Initializer::EngineTerminate() {
        LOG_INFO("Initializer", "Terminating Engine");
        Renderer::Terminate();
        Application::Terminate();
        Input::Terminate();
        ScriptEngine::Terminate();
        ResourceManager::Terminate();
        AssetManager::Terminate();
    }
    void Initializer::DepTerminate() {
        LOG_INFO("Initializer", "Terminating dependencies");
        //* Terminate GLFW
        { glfwTerminate(); }
    }
    void Initializer::PostTerminate() {
        LOG_INFO("Initializer", "PostTerminating");
        Logger::Terminate();
        Config::Terminate();
        Profiler::Terminate();
    }
}  // namespace DE
