#pragma once

#include "DummyEngine/Utils/Base.h"

namespace DE
{

    class SharedObjectImpl;

    class SharedObject
    {
    public:
        SharedObject();
        ~SharedObject();
        bool               Load(const Path& directory, const std::string& name);
        VoidFPtr           GetFunction(const std::string& function_name) const;
        const Path&        GetDirectory() const;
        const std::string& GetName() const;

    private:
        Scope<SharedObjectImpl> m_Impl;
    };
}  // namespace DE