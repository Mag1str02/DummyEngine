#include "ScriptClass.h"

namespace DummyEngine {

    ScriptClass::ScriptClass(const std::string& name) : name_(name) {}
    bool ScriptClass::Load(Ref<SharedObject> library) {
        if (!library->Valid()) {
            return false;
        }
        library_ = library;

        f_create_ = reinterpret_cast<FCreate>(library->GetFunction(name_ + "Create"));
        f_delete_ = reinterpret_cast<FDelete>(library->GetFunction(name_ + "Delete"));
        return Valid();
    }
    bool ScriptClass::Valid() {
        auto ptr = library_.lock();
        if (!ptr) {
            return false;
        }
        return ptr->Valid() && f_create_ != nullptr && f_delete_ != nullptr;
    }
    const std::string& ScriptClass::GetName() const {
        return name_;
    }

}  // namespace DummyEngine