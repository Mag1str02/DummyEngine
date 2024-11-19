#pragma once

#include "DummyEngine/Core/Application/Application.h"
#include "DummyEngine/Core/Application/Initializer.h"
#include "DummyEngine/Utils/Base.h"

namespace DE {
    extern void SetupApplication();
}

int main() {
    try {
        DE::Initializer::Initialize();

        DE::SetupApplication();
        DE::Application::Run();

        DE::Initializer::Terminate();
    } catch (const std::exception& e) {
        LOG_FATAL_AS("EntryPoint", "Unhandled exeption occured: {}", e.what());
        return -1;
    } catch (...) {
        LOG_FATAL_AS("EntryPoint", "Unknown error occured");
        return -1;
    }
    return 0;
}
