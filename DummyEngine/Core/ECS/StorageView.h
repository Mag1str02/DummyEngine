#pragma once

#include "Forward.h"
#include "Signature.h"

namespace DummyEngine {

    template <typename... Components> class StorageView {
    public:
        using Iterator = std::vector<Entity>::const_iterator;

        Iterator begin();  // NOLINT
        Iterator end();    // NOLINT
        bool     Empty();

    private:
        friend class Storage;
        explicit StorageView(Storage* storage);

        std::vector<Entity> view_;
    };

}  // namespace DummyEngine