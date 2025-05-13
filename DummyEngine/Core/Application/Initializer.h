#pragma once

#include "DummyEngine/Utils/Debug/Logger.h"

namespace DummyEngine {

    class Initializer {
        LOG_AUTHOR(Initializer)
    public:
        static void Initialize();
        static void Terminate();

    private:
        static void PreInitialize();
        static void EngineInitialize();

        static void EngineTerminate();
        static void PostTerminate();
    };

}  // namespace DummyEngine