// #include "DummyEngine/Platform/PlatformIncludes.h"
#include "DummyEngine/Core/Scripting/SharedObject.h"

#include <dlfcn.h>

#include "DummyEngine/Core/Application/Config.h"

namespace DummyEngine {
    class SharedObjectImpl {
        LOG_AUTHOR(SharedObject)
    public:
        SharedObjectImpl() {}
        ~SharedObjectImpl() {
            if (handle_) {
                dlclose(handle_);
            }
        }

        SharedObjectImpl(const SharedObjectImpl&)            = delete;
        SharedObjectImpl(SharedObjectImpl&&)                 = delete;
        SharedObjectImpl& operator=(const SharedObjectImpl&) = delete;
        SharedObjectImpl& operator=(SharedObjectImpl&&)      = delete;

        bool Load(const Path& directory, const std::string& name) {
            if (handle_ && valid_) {
                LOG_WARNING("Library {} already loaded", name);
                return false;
            }
            Path path_to_dll = directory / (name + ".dylib");
            if (!fs::exists(path_to_dll)) {
                LOG_WARNING("Library {} does not exist", directory / (name + ".dylib"));
                return false;
            }
            void* new_handle = dlopen(path_to_dll.string().c_str(), RTLD_NOW);
            if (!new_handle) {
                LOG_WARNING("Failed to load library {}", directory / (name + ".dylib"));
                return false;
            }
            if (handle_) {
                dlclose(handle_);
            }
            handle_    = new_handle;
            directory_ = directory;
            name_      = name;
            valid_     = true;
            LOG_INFO("Loaded library {}", Config::RelativeToExecutable(directory / (name + ".dylib")));
            return true;
        }
        void Invalidate() { valid_ = false; }

        bool     Valid() const { return valid_ && handle_; }
        VoidFPtr GetFunction(const std::string& function_name) const {
            if (!handle_) {
                return nullptr;
            }
            return (VoidFPtr)dlsym(handle_, function_name.c_str());
        }
        const Path&        GetDirectory() const { return directory_; }
        const std::string& GetName() const { return name_; }

    private:
        Path        directory_;
        std::string name_;
        void*       handle_ = NULL;
        bool        valid_  = false;
    };

    SharedObject::SharedObject() {
        impl_ = CreateScope<SharedObjectImpl>();
    }
    SharedObject::~SharedObject() {}
    bool SharedObject::Load(const Path& directory, const std::string& name) {
        return impl_->Load(directory, name);
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
    void SharedObject::Invalidate() {
        impl_->Invalidate();
    }
    bool SharedObject::Valid() const {
        return impl_->Valid();
    }

}  // namespace DummyEngine