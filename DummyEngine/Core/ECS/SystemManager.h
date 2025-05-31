#pragma once

#include "Forward.h"

#include <unordered_map>
#include <unordered_set>

namespace DummyEngine {

    class SystemManager {
    public:
        explicit SystemManager(Storage* storage);

        void Update(float dt);

        template <typename Before, typename After> void AddDependency();
        template <typename SystemType> void             AttachSystem(std::shared_ptr<SystemType> system);
        template <typename SystemType> void             DettachSystem();

    private:
        void RefreshOrder();

    private:
        std::unordered_map<std::type_index, U64>         system_ids_;
        std::unordered_map<U64, std::shared_ptr<System>> systems_;
        std::unordered_map<U64, std::unordered_set<U64>> dependency_graph_;
        std::vector<U64>                                 cached_order_;
        U64                                              system_counter_ = 0;

        Storage* storage_;
    };

}  // namespace DummyEngine