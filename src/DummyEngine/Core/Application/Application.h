#pragma once

#include "DummyEngine/Core/Application/Window.h"
#include "DummyEngine/Core/Application/Input.h"
#include "DummyEngine/Core/Application/ImGuiLayer.h"

namespace DE
{

    class Application : public Singleton<Application>
    {
        SINGLETON(Application)
    public:
        S_METHOD_DEF(Unit, Initialize, ());
        S_METHOD_DEF(Unit, Terminate, ());

        S_METHOD_DEF(Unit, PushLayer, (Layer * layer));
        S_METHOD_DEF(Unit, OnEvent, (Event & event));
        S_METHOD_DEF(Unit, Run, ());

    private:
        friend class ImGuiLayer;
        friend class FileSystem;

        Application()  = default;
        ~Application() = default;

        void SetUpCallbacks();
        void OnWindowResize(WindowResizeEvent& e);
        void OnWindowClose(WindowCloseEvent& e);

        bool                m_ShouldClose;
        EventDispatcher     m_EventDispatcher;
        std::vector<Layer*> m_Layers;
        ImGuiLayer*         m_ImGuiLayer;
        Window*             m_Window;
    };
}  // namespace DE