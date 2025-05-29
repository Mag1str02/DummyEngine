#pragma once

#include "Forward.h"
#include "Signature.h"

namespace DummyEngine {

    template <typename... Components>
    class StorageView {
    public:
        StorageView()  = default;
        using Iterator = std::vector<Entity>::const_iterator;

        Iterator begin();  // NOLINT
        Iterator end();    // NOLINT
        bool     Empty();

        const std::vector<Entity>& Entities() const;

    private:
        friend class Storage;
        explicit StorageView(Storage* storage);

        std::vector<Entity> view_;
    };

}  // namespace DummyEngine