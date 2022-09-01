#include "../DummyEngine/Addition/DummyEngine.h"

using namespace DE;

class MyLayer : public Layer
{
public:
    MyLayer() : Layer("MyLayer") {}

    virtual void OnImGuiRender() override
    {
        if (show_demo_window) ImGui::ShowDemoWindow(&show_demo_window);
    }

private:
    bool show_demo_window = true;
};

int main()
{
    Application application("EmptyApplication");
    application.PushLayer(new MyLayer());
    application.Run();
    return 0;
}
