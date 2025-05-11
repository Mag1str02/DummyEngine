#pragma once

#include "DummyEngine/Core/Application/Event.h"
#include "DummyEngine/Utils/Helpers/Singleton.h"

#include <vector>

namespace DummyEngine {

    class Window;
    class ConsoleLayer;
    class ImGuiLayer;
    class Layer;

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

        bool                should_close_;
        EventDispatcher     event_dispatcher_;
        std::vector<Layer*> layers_;
        ImGuiLayer*         imgui_layer_;
        ConsoleLayer*       console_layer_;
        Window*             window_;
    };
}  // namespace DummyEngine