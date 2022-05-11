#pragma once

#include <cinttypes>

namespace DE {
using EntityId = int64_t;

class Entity {
private:
    friend class EntityManager;
    EntityId _id;

    Entity() : _id(-1) {
    }
    Entity(EntityId entity) : _id(entity) {
    }

public:
    EntityId id() {
        return _id;
    }
};
}  // namespace DE
