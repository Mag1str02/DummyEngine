#pragma once

#include "Core/Application/Window.h"
#include "Core/Application/ImGuiLayer.h"
#include "Core/Application/Event.h"

namespace DE
{

    class Application
    {
    public:
        Application(std::string name);
        virtual ~Application();

        Application(const Application&) = delete;
        Application(Application&&) = delete;
        Application& operator=(const Application&) = delete;
        Application& operator=(Application&&) = delete;

        void PushLayer(Layer* layer);
        void OnEvent(Event& event);

        void Run();
 
        static Application& Get();

    protected:
        friend class ImGuiLayer;

        std::vector<Layer*> m_Layers;
        ImGuiLayer* m_ImGuiLayer;
        Window* m_Window;
    };
}  // namespace DE