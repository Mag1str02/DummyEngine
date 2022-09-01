#include "../DummyEngine/Addition/DummyEngine.h"

class MyLayer : public DE::Layer
{
public:
    MyLayer() : DE::Layer("MyLayer") {}

    virtual void OnImGuiRender() override
    {
        if (show_demo_window) ImGui::ShowDemoWindow(&show_demo_window);
    }

private:
    bool show_demo_window = true;
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
