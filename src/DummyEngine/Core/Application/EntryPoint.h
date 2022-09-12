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
    std::cout << "Didn't crash!" << std::endl;
    return 0;
}
