#include "../DummyEngine/Addition/DummyEngine.h"
#include "../DummyEngine/Core/Application/EntryPoint.h"

class MyLayer : public DE::Layer
{
public:
    MyLayer() : DE::Layer("MyLayer") {}
};
namespace DE
{
    Application* CreateApplication()
    {
        Application* app = new Application("MyApplication");
        app->PushLayer(new MyLayer());
        return app;
    }
}  // namespace DE
