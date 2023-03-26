#include "DummyEngine/Core/Application/EntryPoint.h"
#include "DummyEngine/Utils/DummyEngine.h"


class MyLayer : public DE::Layer {
public:
    MyLayer() : DE::Layer("MyLayer") {}
};
namespace DE {

    void SetupApplication() {
        Application::PushLayer(new MyLayer());
    }
}  // namespace DE
