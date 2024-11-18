#pragma once

#include "DummyEngine/Utils/Base.h"

namespace DE {
    class Initializer {
        LOGGER_AUTHOR(Initializer)
    public:
        static void Initialize();
        static void Terminate();

    private:
        static void PreInitialize();
        static void DepInitialize();
        static void EngineInitialize();

        static void EngineTerminate();
        static void DepTerminate();
        static void PostTerminate();
    };
}  // namespace DE