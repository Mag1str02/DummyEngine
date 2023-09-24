#include "DummyEngine/Core/Scripting/ScriptClass.h"

namespace DE {
    ScriptClass::ScriptClass(const std::string& name) : m_Name(name) {}
    bool ScriptClass::Load(Ref<SharedObject> library) {
        if (!library->Valid()) {
            return false;
        }
        m_Library = library;

        f_create = reinterpret_cast<FCreate>(library->GetFunction(m_Name + "Create"));
        f_delete = reinterpret_cast<FDelete>(library->GetFunction(m_Name + "Delete"));
        return Valid();
    }
    bool ScriptClass::Valid() {
        auto ptr = m_Library.lock();
        if (!ptr) {
            return false;
        }
        return ptr->Valid() && f_create && f_delete;
    }
    const std::string& ScriptClass::GetName() const {
        return m_Name;
    }
}  // namespace DE