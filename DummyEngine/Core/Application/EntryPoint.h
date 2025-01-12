#pragma once

#include "DummyEngine/Core/Application/Application.h"
#include "DummyEngine/Core/Application/Initializer.h"

namespace DummyEngine {
    extern void SetupApplication();
}

int main() {
    try {
        DummyEngine::Initializer::Initialize();

        DummyEngine::SetupApplication();
        DummyEngine::Application::Run();

        DummyEngine::Initializer::Terminate();
    } catch (const std::exception& e) {
        LOG_FATAL_AS("EntryPoint", "Unhandled exeption occured: {}", e.what());
        return -1;
    } catch (...) {
        LOG_FATAL_AS("EntryPoint", "Unknown error occured");
        return -1;
    }
    return 0;
}
