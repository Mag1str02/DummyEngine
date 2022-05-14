#pragma once

#include <cinttypes>

#define H_MAX_ENTITY_AMOUNT 1
#define H_MAX_COMPONENT_AMOUNT 2

namespace DE
{

    class Initializer
    {
    private:
        friend class EntityManager;
        template <typename ComponentType>
        friend class ComponentArray;

        int64_t _max_entity_amount;
        int64_t _max_component_amount;

        Initializer();

    public:
        static Initializer &Get();

        void SetHint(int64_t hint_id, int64_t hint_value);
    };
    void deInitialize();
    void deTerminate();

    void deHint(int64_t hint_id, int64_t hint_value);
} // namespace DE
