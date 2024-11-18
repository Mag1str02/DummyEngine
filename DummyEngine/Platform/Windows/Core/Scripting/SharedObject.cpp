#include "DummyEngine/Core/Scripting/SharedObject.h"

#include <windows.h>

#include "DummyEngine/Core/Application/Config.h"

namespace DE {
    class SharedObjectImpl {
        LOGGER_AUTHOR(SharedObject)
    public:
        SharedObjectImpl() {}
        ~SharedObjectImpl() {
            if (m_Handle) {
                FreeLibrary(m_Handle);
            }
        }

        SharedObjectImpl(const SharedObjectImpl&)            = delete;
        SharedObjectImpl(SharedObjectImpl&&)                 = delete;
        SharedObjectImpl& operator=(const SharedObjectImpl&) = delete;
        SharedObjectImpl& operator=(SharedObjectImpl&&)      = delete;

        bool Load(const Path& directory, const std::string& name) {
            if (m_Handle && m_Valid) {
                LOG_WARNING("Library {} already loaded", name);
                return false;
            }
            Path path_to_dll = directory / (name + ".dll");
            if (!fs::exists(path_to_dll)) {
                LOG_WARNING("Library {} does not exist", path_to_dll);
                return false;
            }
            HMODULE new_handle = LoadLibrary(path_to_dll.string().c_str());
            if (!new_handle) {
                LOG_WARNING("Failed to load library {}", path_to_dll);
                return false;
            }
            if (m_Handle) {
                FreeLibrary(m_Handle);
            }

            m_Handle    = new_handle;
            m_Directory = directory;
            m_Name      = name;
            m_Valid     = true;
            LOG_INFO("Loaded library {}", RelativeToExecutable(path_to_dll));
            return true;
        }
        void Invalidate() { m_Valid = false; }

        bool     Valid() const { return m_Valid && m_Handle; }
        VoidFPtr GetFunction(const std::string& function_name) const {
            if (!m_Handle) {
                return nullptr;
            }
            return (VoidFPtr)GetProcAddress(m_Handle, function_name.c_str());
        }
        const Path&        GetDirectory() const { return m_Directory; }
        const std::string& GetName() const { return m_Name; }

    private:
        Path        m_Directory;
        std::string m_Name;
        HMODULE     m_Handle = NULL;
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