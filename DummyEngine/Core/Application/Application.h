#pragma once

#include "DummyEngine/Core/Application/ImGuiLayer.h"
#include "DummyEngine/Core/Application/Input.h"
#include "DummyEngine/Core/Application/Window.h"

namespace DE {

    class Application : public Singleton<Application> {
        SINGLETON(Application)
    public:
        S_METHOD_DEF(Unit, PushLayer, (Layer * layer));
        S_METHOD_DEF(Unit, OnEvent, (Event & event));
        S_METHOD_DEF(Unit, Run, ());
        S_METHOD_DEF(Window&, GetWindow, ());

    private:
        friend class ImGuiLayer;
        friend class FileSystem;

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