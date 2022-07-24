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

    public:
        static Initializer& Get();
        void SetHint(int64_t hint_id, int64_t hint_value);
        void Initialize();
        void Terminate();
    };
    void deInitialize();
    void deTerminate();

    void deHint(int64_t hint_id, int64_t hint_value);
}  // namespace DE
