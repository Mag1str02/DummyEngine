#include "DummyEngine/Core/Scripting/ScriptClass.h"

namespace DE
{
    ScriptClass::ScriptClass(const std::string& name) : m_Name(name) {}
    bool ScriptClass::Load(Ref<SharedObject> library)
    {
        if (!library->Valid())
        {
            return false;
        }
        m_Library = library;

        constructor     = reinterpret_cast<FConstructor>(library->GetFunction(m_Name + "Construct"));
        destructor      = reinterpret_cast<FDestructor>(library->GetFunction(m_Name + "Destruct"));
        on_create       = reinterpret_cast<FOnCreate>(library->GetFunction(m_Name + "OnCreate"));
        on_update       = reinterpret_cast<FOnUpdate>(library->GetFunction(m_Name + "OnUpdate"));
        on_destroy      = reinterpret_cast<FOnDestroy>(library->GetFunction(m_Name + "OnDestroy"));
        align_of        = reinterpret_cast<FAlignOf>(library->GetFunction(m_Name + "AlignOf"));
        size_of         = reinterpret_cast<FSizeOf>(library->GetFunction(m_Name + "SizeOf"));
        create_instance = reinterpret_cast<FCreateInstance>(library->GetFunction(m_Name + "CreateInstance"));
        return Valid();
    }
    bool ScriptClass::Valid()
    {
        auto ptr = m_Library.lock();
        if (!ptr)
        {
            return false;
        }
        return ptr->Valid() && constructor && destructor && on_create && on_update && on_destroy && align_of && size_of && create_instance;
    }
}  // namespace DE