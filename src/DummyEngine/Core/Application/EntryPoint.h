#pragma once

#include "DummyEngine/Core/Application/Application.h"
#include "DummyEngine/Core/Application/Initializer.h"
#include "DummyEngine/Utils/Base.h"

namespace DE
{
    extern void SetupApplication();
}

int main()
{
    try
    {
        DE::Initializer::Initialize();

        DE::SetupApplication();
        DE::Application::Run();

        DE::Initializer::Terminate();
    } catch (const std::exception& e)
    {
        DE::Logger::Fatal(std::string("Unhandled exeption occured: ") + e.what(), "EntryPoint");
        return -1;
    } catch (...)
    {
        DE::Logger::Fatal(std::string("Unknown error occured"), "EntryPoint");
        return -1;
    }
    return 0;
}
