#pragma once

#include "Forward.h"
#include "Signature.h"

namespace DummyEngine {

    template <typename... Components> class StorageView {
    public:
        class Iterator {
        public:
            Iterator(U32 id, StorageView* v);

            bool      operator==(const Iterator& other) const;
            bool      operator!=(const Iterator& other) const;
            Iterator& operator++();
            Iterator  operator++(int);
            Entity    operator*();

        private:
            friend class StorageView;
            U32          id_;
            StorageView* view_;
        };
        Iterator begin();  // NOLINT
        Iterator end();    // NOLINT
        bool     Empty();

    private:
        friend class Storage;
        explicit StorageView(Storage* storage);

        Signature signature_;
        Storage*  storage_;
    };

}  // namespace DummyEngine