#include "DummyEditor/EditorLayer.h"
#include "DummyEngine/Core/Application/Application.h"
#include "DummyEngine/Core/Application/Engine.h"

namespace DummyEngine {
    void SetupApplication() {
        Application::PushLayer(new EditorLayer());
    }
}  // namespace DummyEngine

int main() {
    DummyEngine::Main(DummyEngine::SetupApplication);
    return 0;
}