#include "DummyEngine/Core/Application/EntryPoint.h"
#include "DummyEngine/DummyEngine.h"

class MyLayer : public DummyEngine::Layer {
public:
    MyLayer() : DummyEngine::Layer("MyLayer") {}
};
namespace DummyEngine {

    void SetupApplication() {
        Application::PushLayer(new MyLayer());
    }
}  // namespace DummyEngine
