#include "DummyEngine/Core/Scripting/SharedObject.h"

namespace DE {
    class SharedObjectImpl {
    public:
        SharedObjectImpl() {}
        ~SharedObjectImpl() {}

        SharedObjectImpl(const SharedObjectImpl&)            = delete;
        SharedObjectImpl(SharedObjectImpl&&)                 = delete;
        SharedObjectImpl& operator=(const SharedObjectImpl&) = delete;
        SharedObjectImpl& operator=(SharedObjectImpl&&)      = delete;

        bool Load(const Path& directory, const std::string& name) {
            return true;
        }
        void Invalidate() { m_Valid = false; }

        bool     Valid() const { return m_Valid; }
        VoidFPtr GetFunction(const std::string& function_name) const {
            return nullptr;
        }
        const Path&        GetDirectory() const { return m_Directory; }
        const std::string& GetName() const { return m_Name; }

    private:
        Path        m_Directory;
        std::string m_Name;
//        HMODULE     m_Handle = NULL;
        bool        m_Valid  = false;
    };

    SharedObject::SharedObject() {
        m_Impl = CreateScope<SharedObjectImpl>();
    }
    SharedObject::~SharedObject() {}
    bool SharedObject::Load(const Path& directory, const std::string& name) {
        return m_Impl->Load(directory, name);
    }
    void SharedObject::Invalidate() {
        m_Impl->Invalidate();
    }

    bool SharedObject::Valid() const {
        return m_Impl->Valid();
    }
    VoidFPtr SharedObject::GetFunction(const std::string& function_name) const {
        return m_Impl->GetFunction(function_name);
    }
    const Path& SharedObject::GetDirectory() const {
        return m_Impl->GetDirectory();
    }
    const std::string& SharedObject::GetName() const {
        return m_Impl->GetName();
    }

}  // namespace DE