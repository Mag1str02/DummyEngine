#include "Core/Wrappings/Application.h"

#include "Core/ECS/SystemManager.hpp"
#include "Core/Initializer/initializer.h"
#include "Core/Rendering/Renderer/Renderer.h"
#include "ToolBox/Dev/FrameTimeReader.h"

namespace DE
{

    Application::Application(std::string name)
    {
        Config::Init();
        Logger::Open(Config::GetPath(DE_CFG_LOG_PATH) / "loading.txt", "loading");
        Logger::Open(Config::GetPath(DE_CFG_LOG_PATH) / "ECS.txt", "ECS");
        deInitialize();
        m_Window = new Window();
        m_Window->SetName(name);
    }
    Application::~Application()
    {
        delete m_Window;

        Logger::Close("ECS");
        Logger::Close("loading");
        deTerminate();
    }

    void Application::OnLoad() {}
    void Application::OnClose() {}

    void Application::Start()
    {
        m_Window->Init();
        Renderer::Init(Config::GetRenderAPI());
        Renderer::Load(m_Window);
        OnLoad();
        double frame_begin, frame_end, prev_frame_time = 0.001, fake_time;
        while (!m_Window->ShouldClose())
        {
            frame_begin = glfwGetTime();
            DE_PROFILE_SCOPE("", {
                Renderer::BeginFrame();

                DE_PROFILE_SCOPE("Poll Events", glfwPollEvents());
                DE_PROFILE_SCOPE("Application Update Function", Update(prev_frame_time));
                DE_PROFILE_SCOPE("Buffer Swap", m_Window->SwapBuffers());

                Renderer::EndFrame();
            });

            DE_FTR_PRINT();

            frame_end = glfwGetTime();
            prev_frame_time = frame_end - frame_begin;
        }
        OnClose();
    }
    void Application::Update(double dt) {}

}  // namespace DE
