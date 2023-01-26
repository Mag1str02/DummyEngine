#include "DummyEditor/EditorLayer.h"
#include "DummyEngine/Core/Application/EntryPoint.h"

namespace DE
{
    void SetupApplication() { Application::PushLayer(new EditorLayer()); }
}  // namespace DE
