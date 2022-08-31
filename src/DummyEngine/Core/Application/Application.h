#pragma once

#include "Core/Application/Window.h"

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

        void Run();
        virtual void OnLoad();
        virtual void OnUpdate(double dt);
        virtual void OnShutdown();

        static Application& Get();

    protected:
        friend class ImGuiLayer;

        Scope<Window> m_Window;
    };
}  // namespace DE