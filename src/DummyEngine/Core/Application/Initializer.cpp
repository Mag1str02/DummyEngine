#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "DummyEngine/Utils/Base.h"
#include "DummyEngine/Core/Application/Initializer.h"
#include "DummyEngine/Core/Application/Application.h"
#include "DummyEngine/Core/Rendering/Renderer/Renderer.h"
#include "DummyEngine/Core/Scripting/ScriptEngine.h"

namespace DE
{

    void Initializer::Initialize()
    {
        PreInitialize();
        DepInitialize();
        EngineInitialize();
    }
    void Initializer::Terminate()
    {
        EngineTerminate();
        DepTerminate();
        PostTerminate();
    }

    void Initializer::PreInitialize()
    {
        Profiler::Initialize();
        Config::Initialize();
        Logger::Initialize();
        LOG_INFO("Logger and config initialized", "Initializer");
    }
    void Initializer::DepInitialize()
    {
        LOG_STAGE("Initializing dependencies", "Initializer");
        //* Init GLFW
        {
            if (!glfwInit())
            {
                LOG_FATAL("Failed to initialize GLFW", "Initializer");
                DE_ASSERT(false, "Failed to initialize GLFW");
            }
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            LOG_INFO("Initialized GLFW", "Initializer");
        }
    }
    void Initializer::EngineInitialize()
    {
        DE_PROFILER_BEGIN_FRAME();

        LOG_STAGE("Initializing Engine", "Initializer");
        ScriptEngine::Initialize();
        Input::Initialize();
        Application::Initialize();
        Renderer::Initialize();
    }

    void Initializer::EngineTerminate()
    {
        LOG_STAGE("Terminating Engine", "Initializer");
        Renderer::Terminate();
        Application::Terminate();
        Input::Terminate();
        ScriptEngine::Terminate();
    }
    void Initializer::DepTerminate()
    {
        LOG_STAGE("Terminating dependencies", "Initializer");
        //* Terminate GLFW
        {
            glfwTerminate();
        }
    }
    void Initializer::PostTerminate()
    {
        LOG_STAGE("PostTerminating", "Initializer");
        Logger::Terminate();
        Config::Terminate();
        Profiler::Terminate();
    }
}  // namespace DE
