#include "EditorLayer.h"
#include "Core/Application/EntryPoint.h"

namespace DE
{
    Application* CreateApplication()
    {
        Application* app = new Application("DummyEditor");
        app->PushLayer(new EditorLayer());
        return app;
    }
}  // namespace DE
