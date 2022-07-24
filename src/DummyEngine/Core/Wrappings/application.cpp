#include "DummyEngine/Core/Wrappings/Application.h"
#include "DummyEngine/ToolBox/Dev/FrameTimeReader.h"
#include "DummyEngine/Core/ECS/SystemManager.hpp"
#include "DummyEngine/Core/Rendering/Renderer/Renderer.h"

namespace DE {

Application::Application(std::string name) {
    Logger::Open(LOG_DIR / "loading.txt", "loading");
    Logger::Open(LOG_DIR / "ECS.txt", "ECS");
    InitGLFW();
    deInitialize();
    m_Window = new Window();
    m_Window->SetName(name);
}
Application::~Application() {
    delete m_Window;

    Logger::Info("loading", "Main", "Loading time: " + std::to_string(glfwGetTime()) + "s");
    Logger::Close("ECS");
    Logger::Close("loading");
    deTerminate();
    glfwTerminate();
}

void Application::OnLoad() {
}

void Application::Start() {
    m_Window->Init();
    Renderer::Init(API::OpenGL);
    Renderer::Load(m_Window);
    OnLoad();
    double frame_begin, frame_end, prev_frame_time = 0.001;
    while (!m_Window->ShouldClose()) {
        DE_FTR_ENTER("");
        frame_begin = glfwGetTime();

        DE_FTR_ENTER("Poll Events");
        glfwPollEvents();
        DE_FTR_LEAVE();

        DE_FTR_ENTER("Application Update Function");
        Update(prev_frame_time);
        DE_FTR_LEAVE();

        DE_FTR_ENTER("System Processing");
        SystemManager::Update(prev_frame_time);
        DE_FTR_LEAVE();

        DE_FTR_ENTER("Frame Swap");
        m_Window->SwapBuffers();
        DE_FTR_LEAVE();

        frame_end = glfwGetTime();
        prev_frame_time = frame_end - frame_begin;
        DE_FTR_LEAVE();
        DE_FTR_PRINT();
    }
}
void Application::Update(double dt) {
}
bool Application::InitGLFW() {
    if (!glfwInit()) {
        Logger::Fatal("loading", "Application", "Failed to initialize GLFW.");
        return false;
    }
    SetDefaultGLFWSettings();
    Logger::Info("loading", "Application", "GLFW initialized.");
    return true;
}

void Application::SetDefaultGLFWSettings() {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);
}

}  // namespace DE
   /*     while (!glfwWindowShouldClose(m_Window)) {
              double prev_frame_time = glfwGetTime() - prev_time;
              prev_time = glfwGetTime();
              DE_FTR_ENTER("");
              DE_FTR_ENTER("Poll Events");
              glfwPollEvents();
              DE_FTR_LEAVE();
              DE_FTR_ENTER("Process Input");
              _process_input();
              DE_FTR_LEAVE();
              DE_FTR_ENTER("System Processing");
              SystemManager::Update(prev_frame_time);
              DE_FTR_LEAVE();
              DE_FTR_ENTER("Frame Swap");
              glfwSwapBuffers(m_Window);
              max_frame_time = std::max(max_frame_time, glfwGetTime() - prev_time);
              avarage_frame_time = (avarage_frame_time * frame_amount + glfwGetTime() - prev_time) / (frame_amount + 1);
              frame_amount++;
              DE_FTR_LEAVE();
              DE_FTR_LEAVE();
              DE_FTR_PRINT();
          }
          Logger::Info("rendering", "Window", "Avarage frame time: " + std::to_string(avarage_frame_time) + "s" + " FPS: " + std::to_string(1.0 / avarage_frame_time));
          Logger::Info("rendering", "Window", "Avarage frame time: " + std::to_string(max_frame_time) + "s" + " FPS: " + std::to_string(1.0 / max_frame_time));
      */