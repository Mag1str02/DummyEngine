#include "DummyEngine/Core/Wrappings/Application.h"
#include "DummyEngine/ToolBox/Dev/FrameTimeReader.h"
#include "DummyEngine/Core/ECS/SystemManager.hpp"
#include "DummyEngine/Core/Rendering/Renderer/Renderer.h"

namespace DE
{

    Application::Application(std::string name)
    {
        Config::Init();
        Logger::Open(Config::GetPath(DE_CFG_EXECUTABLE_PATH) / "loading.txt", "loading");
        Logger::Open(Config::GetPath(DE_CFG_EXECUTABLE_PATH) / "ECS.txt", "ECS");
        deInitialize();
        m_Window = new Window();
        m_Window->SetName(name);
    }
    Application::~Application()
    {
        delete m_Window;

        Logger::Info("loading", "Main", "Loading time: " + std::to_string(glfwGetTime()) + "s");
        Logger::Close("ECS");
        Logger::Close("loading");
        deTerminate();
    }

    void Application::OnLoad() {}

    void Application::Start()
    {
        m_Window->Init();
        Renderer::Init(API::OpenGL);
        Renderer::Load(m_Window);
        OnLoad();
        double frame_begin, frame_end, prev_frame_time = 0.001;
        while (!m_Window->ShouldClose())
        {
            DE_PROFILE_SCOPE("", {
                frame_begin = glfwGetTime();

                DE_PROFILE_SCOPE("Poll Events", glfwPollEvents());
                DE_PROFILE_SCOPE("Application Update Function", Update(prev_frame_time));
                DE_PROFILE_SCOPE("System Processing", SystemManager::Update(prev_frame_time));
                DE_PROFILE_SCOPE("Buffer Swap", m_Window->SwapBuffers());

                frame_end = glfwGetTime();
                prev_frame_time = frame_end - frame_begin;
            });

            DE_FTR_PRINT();
        }
    }
    void Application::Update(double dt) {}

}  // namespace DE
