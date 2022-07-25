#pragma once

#include "DummyEngine/Addition/Base.h"

namespace DE
{
    class Initializer
    {
    private:
        template <typename ComponentType>
        friend class ComponentArray;
        friend class EntityManager;

        Initializer();

        void InitGLFW();
        void TerminateGLFW();

    public:
        static Initializer& Get();
        void Initialize();
        void Terminate();
    };
    void deInitialize();
    void deTerminate();

}  // namespace DE
