#pragma once

#include "Forward.h"

namespace DummyEngine {

    class System {
    public:
        virtual ~System()                    = default;
        virtual void        Update(float dt) = 0;
        virtual std::string GetName() const  = 0;

    protected:
        template <typename... Components> StorageView<Components...> View();

    private:
        friend class SystemManager;
        void Bind(Storage* storage);

        Storage* storage_;
    };

}  // namespace DummyEngine