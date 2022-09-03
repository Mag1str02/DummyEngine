#include "Core/Application/Application.h"

namespace DE
{
    Application* CreateApplication();
}

int main()
{
    auto app = DE::CreateApplication();
    app->Run();
    delete app;
}
