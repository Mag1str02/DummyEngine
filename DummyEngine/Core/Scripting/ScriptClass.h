#pragma once

#include "DummyEngine/Core/Scripting/Script.h"
#include "DummyEngine/Core/Scripting/SharedObject.h"

namespace DummyEngine {

    class ScriptClass {
    public:
        using FCreate = Script* (*)();
        using FDelete = void (*)(Script*);

        ScriptClass() = default;
        explicit ScriptClass(const std::string& name);

        bool               Load(Ref<SharedObject> library);
        bool               Valid();
        const std::string& GetName() const;

        inline Script* Create() { return f_create_(); }
        inline void    Delete(Script* ptr) { f_delete_(ptr); }

    private:
        std::string           name_;
        WeakRef<SharedObject> library_;

        FCreate f_create_ = nullptr;
        FDelete f_delete_ = nullptr;
    };
}  // namespace DummyEngine
