#pragma once

namespace DummyEngine {
#ifdef ECS_IMPLEMENTATION
    std::pair<U32, U32> EntityManager::CreateEntity() {
        if (available_entities_.empty()) {
            available_entities_.push(states_.size());
            states_.push_back(false);
            generations_.push_back(0);
        }
        auto id = available_entities_.front();
        available_entities_.pop();
        states_[id] = true;
        ++generations_[id];
        return std::make_pair(id, generations_[id]);
    }
    void EntityManager::Destroy(U32 id) {
        states_[id] = false;
        available_entities_.push(id);
    }
    bool EntityManager::Valid(U32 id, U32 gen) const {
        return states_[id] && generations_[id] == gen;
    }

    U32 EntityManager::NextEntity(U32 id) const {
        if (id >= states_.size()) {
            return states_.size();
        }
        do {
            ++id;
        } while (id < states_.size() && !states_[id]);
        return id;
    }
    U32 EntityManager::Generation(U32 id) const {
        return generations_[id];
    }
    U32 EntityManager::BeginEntity() const {
        U32 res = 0;
        while (res < states_.size() && !states_[res]) {
            ++res;
        }
        return res;
    }
    U32 EntityManager::EndEntity() const {
        return states_.size();
    }
#endif
}  // namespace DummyEngine