#pragma once

namespace DE
{
    class Initializer
    {
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