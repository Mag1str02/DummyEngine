#pragma once

#include "Core/Application/Application.h"

namespace DE
{
    extern Application* CreateApplication();
}

int main()
{
    auto app = DE::CreateApplication();
    app->Run();
    delete app;
    return 0;
}
