#pragma once

#include "DummyEngine/Core/Application/Window.h"
#include "DummyEngine/Core/Application/Input.h"
#include "DummyEngine/Core/Application/ImGuiLayer.h"

namespace DE
{

    class Application
    {
    public:
        Application(std::string name);
        ~Application();

        Application(const Application&) = delete;
        Application(Application&&) = delete;
        Application& operator=(const Application&) = delete;
        Application& operator=(Application&&) = delete;

        void PushLayer(Layer* layer);
        void OnEvent(Event& event);

        void Run();

        static Application& Get();

    private:
        friend class ImGuiLayer;
        friend class FileDialogs;

        void SetUpCallbacks();
        void OnWindowResize(WindowResizeEvent& e);
        void OnWindowClose(WindowCloseEvent& e);

        bool m_ShouldClose;
        EventDispatcher m_EventDispatcher;
        std::vector<Layer*> m_Layers;
        ImGuiLayer* m_ImGuiLayer;
        Window* m_Window;
    };
}  // namespace DE