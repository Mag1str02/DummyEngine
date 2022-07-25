#include <GLFW/glfw3.h>

#include "Core/Initializer/Initializer.h"
#include "Core/ECS/ComponentManager.hpp"
#include "Core/ECS/EntityManager.hpp"
#include "Core/ECS/SystemManager.hpp"

namespace DE
{

    Initializer::Initializer() {}
    Initializer& Initializer::Get()
    {
        static Initializer initializer;
        return initializer;
    }

    void Initializer::InitGLFW()
    {
        if (!glfwInit())
        {
            DE_ASSERT(false, "Failed to initialize GLFW.");
        }
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        Logger::Info("loading", "Initializer", "GLFW initialized.");
    }
    void Initializer::TerminateGLFW()
    {
        glfwTerminate();
    }

    void Initializer::Initialize()
    {
        InitGLFW();
        EntityManager::Get().Initialize();
        ComponentManager::Get();
    }
    void Initializer::Terminate()
    {
        EntityManager::Get().Terminate();
        ComponentManager::Get().Terminate();
        SystemManager::Terminate();
        TerminateGLFW();
    }

    void deInitialize()
    {
        Initializer::Get().Initialize();
    }
    void deTerminate()
    {
        Initializer::Get().Terminate();
    }
}  // namespace DE
