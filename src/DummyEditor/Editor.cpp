#include "EditorLayer.h"
#include "Core/Application/EntryPoint.h"

namespace DE
{
    void SetupApplication() { Application::PushLayer(new EditorLayer()); }
}  // namespace DE
