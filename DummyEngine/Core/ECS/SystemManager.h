#pragma once

#include "Forward.h"

namespace DummyEngine {

    class SystemManager {
    public:
        explicit SystemManager(Storage* storage);
        void                                            Update(float dt);
        template <typename Before, typename After> void AddDependency();
        template <typename SystemType> void             AttachSystem(std::shared_ptr<System> system);

    private:
        std::vector<std::shared_ptr<System>>     systems_;
        std::unordered_map<std::type_index, U32> system_ids_;
        std::vector<std::vector<U32>>            dependency_graph_;
        Storage*                                 storage_;
    };

}  // namespace DummyEngine