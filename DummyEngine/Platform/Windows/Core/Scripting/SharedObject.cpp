#include "DummyEngine/Core/Scripting/SharedObject.h"

#include "DummyEngine/Core/Application/Config.h"
#include "DummyEngine/Utils/Debug/Logger.h"

#include <windows.h>

namespace DummyEngine {
    class SharedObjectImpl {
        LOG_AUTHOR(SharedObject)
    public:
        SharedObjectImpl() {}
        ~SharedObjectImpl() {
            if (handle_ != nullptr) {
                FreeLibrary(handle_);
            }
        }

        SharedObjectImpl(const SharedObjectImpl&)            = delete;
        SharedObjectImpl(SharedObjectImpl&&)                 = delete;
        SharedObjectImpl& operator=(const SharedObjectImpl&) = delete;
        SharedObjectImpl& operator=(SharedObjectImpl&&)      = delete;

        bool Load(const Path& directory, const std::string& name) {
            if (handle_ != nullptr && valid_) {
                LOG_WARNING("Library {} already loaded", name);
                return false;
            }
            Path path_to_dll = directory / (name + ".dll");
            if (!fs::exists(path_to_dll)) {
                LOG_WARNING("Library {} does not exist", path_to_dll);
                return false;
            }
            HMODULE new_handle = LoadLibrary(path_to_dll.string().c_str());
            if (new_handle == nullptr) {
                LOG_WARNING("Failed to load library {}", path_to_dll);
                return false;
            }
            if (handle_ != nullptr) {
                FreeLibrary(handle_);
            }

            handle_    = new_handle;
            directory_ = directory;
            name_      = name;
            valid_     = true;
            LOG_INFO("Loaded library {}", Config::RelativeToExecutable(path_to_dll));
            return true;
        }
        void Invalidate() { valid_ = false; }

        bool     Valid() const { return valid_ && handle_ != nullptr; }
        VoidFPtr GetFunction(const std::string& function_name) const {
            if (handle_ == nullptr) {
                return nullptr;
            }
            return (VoidFPtr)GetProcAddress(handle_, function_name.c_str());
        }
        const Path&        GetDirectory() const { return directory_; }
        const std::string& GetName() const { return name_; }

    private:
        Path        directory_;
        std::string name_;
        HMODULE     handle_ = nullptr;
        bool        valid_  = false;
    };

    SharedObject::SharedObject() {
        impl_ = CreateScope<SharedObjectImpl>();
    }
    SharedObject::~SharedObject() {}
    bool SharedObject::Load(const Path& directory, const std::string& name) {
        return impl_->Load(directory, name);
    }
    void SharedObject::Invalidate() {
        impl_->Invalidate();
    }

    bool SharedObject::Valid() const {
        return impl_->Valid();
    }
    VoidFPtr SharedObject::GetFunction(const std::string& function_name) const {
        return impl_->GetFunction(function_name);
    }
    const Path& SharedObject::GetDirectory() const {
        return impl_->GetDirectory();
    }
    const std::string& SharedObject::GetName() const {
        return impl_->GetName();
    }

}  // namespace DummyEngine