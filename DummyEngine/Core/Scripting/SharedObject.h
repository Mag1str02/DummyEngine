#pragma once

#include "DummyEngine/Utils/Types/Types.h"

namespace DummyEngine {

    class SharedObjectImpl;

    class SharedObject {
    public:
        SharedObject();
        ~SharedObject();

        bool Load(const Path& directory, const std::string& name);
        void Invalidate();

        bool               Valid() const;
        VoidFPtr           GetFunction(const std::string& function_name) const;
        const Path&        GetDirectory() const;
        const std::string& GetName() const;

    private:
        Scope<SharedObjectImpl> impl_;
    };

}  // namespace DummyEngine