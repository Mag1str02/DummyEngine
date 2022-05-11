#pragma once

#include <cinttypes>

#define MAX_ENTITY_AMOUNT 1

namespace DE {

class Initializer {
private:
    friend class EntityManager;

    int64_t _max_entity_amount;
    Initializer();

public:
    static Initializer& Get();

    void SetHint(uint64_t hint_id, int64_t hint_value);
};

void deHint(int64_t hint_id, int64_t hint_value);
}  // namespace DE
