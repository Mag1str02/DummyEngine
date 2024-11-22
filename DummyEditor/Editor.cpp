#include "DummyEditor/EditorLayer.h"
#include "DummyEngine/Core/Application/EntryPoint.h"

namespace DummyEngine {
    void SetupApplication() {
        Application::PushLayer(new EditorLayer());
    }
}  // namespace DummyEngine
