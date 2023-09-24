#pragma once

#include "DummyEngine/Core/Scripting/Script.h"
#include "DummyEngine/Core/Scripting/SharedObject.h"
#include "DummyEngine/Utils/Base.h"

namespace DE {

    class ScriptClass {
    public:
        using FCreate = Script* (*)();
        using FDelete = void (*)(Script*);

        ScriptClass() = default;
        ScriptClass(const std::string& name);

        bool               Load(Ref<SharedObject> library);
        bool               Valid();
        const std::string& GetName() const;

        inline Script* Create() { return f_create(); }
        inline void    Delete(Script* ptr) { f_delete(ptr); }

    private:
        std::string           m_Name;
        WeakRef<SharedObject> m_Library;

        FCreate f_create = nullptr;
        FDelete f_delete = nullptr;
    };
}  // namespace DE
