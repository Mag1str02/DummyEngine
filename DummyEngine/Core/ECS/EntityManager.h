#pragma once

#include "DummyEngine/Utils/Types/Types.h"

#include <queue>
#include <vector>

namespace DummyEngine {

    class EntityManager {
    public:
        EntityManager() = default;

        std::pair<U32, U32> CreateEntity();
        void                Destroy(U32 id);
        bool                Valid(U32 id, U32 gen) const;

        U32 Generation(U32 id) const;
        U32 NextEntity(U32 id) const;
        U32 BeginEntity() const;
        U32 EndEntity() const;

    private:
        std::vector<U32>  generations_;
        std::vector<bool> states_;
        std::queue<U32>   available_entities_;
    };

}  // namespace DummyEngine