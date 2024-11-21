#include "ScriptEngine.h"

#include "DummyEngine/Core/ResourceManaging/AssetManager.h"
#include "DummyEngine/Core/Scene/Scene.h"

namespace DummyEngine {
    SINGLETON_BASE(ScriptEngine);

    ScriptComponent::ScriptComponent(U32 id) : id_(id) {
        ScriptEngine::IncreaseRefCount(id_);
    }
    ScriptComponent::ScriptComponent(const ScriptComponent& other) {
        id_ = other.id_;
        if (other.id_ != UINT32_MAX) {
            ScriptEngine::IncreaseRefCount(id_);
        }
    }
    ScriptComponent::ScriptComponent(ScriptComponent&& other) {
        id_       = other.id_;
        other.id_ = UINT32_MAX;
    }
    ScriptComponent& ScriptComponent::operator=(const ScriptComponent& other) {
        if (&other != this) {
            if (id_ != UINT32_MAX) {
                ScriptEngine::DecreaseRefCount(id_);
            }
            id_ = other.id_;
            if (other.id_ != UINT32_MAX) {
                ScriptEngine::IncreaseRefCount(id_);
            }
        }
        return *this;
    }
    ScriptComponent& ScriptComponent::operator=(ScriptComponent&& other) {
        if (&other != this) {
            if (id_ != UINT32_MAX) {
                ScriptEngine::DecreaseRefCount(id_);
            }
            id_       = other.id_;
            other.id_ = UINT32_MAX;
        }
        return *this;
    }
    ScriptComponent::~ScriptComponent() {
        if (id_ != UINT32_MAX) {
            ScriptEngine::DecreaseRefCount(id_);
        }
    }
    UUID ScriptComponent::ID() const {
        return ScriptEngine::GetInstance().proxy_manager_.GetProxy(id_).ScriptID;
    }
    bool ScriptComponent::Valid() const {
        return id_ != UINT32_MAX && ScriptEngine::GetInstance().proxy_manager_.GetProxy(id_).Script != nullptr;
    }
    Script& ScriptComponent::operator*() {
        return *ScriptEngine::GetInstance().proxy_manager_.GetProxy(id_).Script;
    }
    Script* ScriptComponent::operator->() {
        return ScriptEngine::GetInstance().proxy_manager_.GetProxy(id_).Script;
    }

    bool ScriptProxyManager::Iterator::operator==(const Iterator& other) const {
        return manager_ == other.manager_ && id_ == other.id_;
    }
    bool ScriptProxyManager::Iterator::operator!=(const Iterator& other) const {
        return id_ != other.id_ || manager_ != other.manager_;
    }

    ScriptProxyManager::Iterator& ScriptProxyManager::Iterator::operator++() {
        do {
            ++id_;
        } while (id_ < manager_->states_.size() && !manager_->states_[id_]);
        return *this;
    }
    ScriptProxyManager::Iterator ScriptProxyManager::Iterator::operator++(int) {
        Iterator res = *this;
        ++(*this);
        return res;
    }
    ScriptProxy& ScriptProxyManager::Iterator::operator*() {
        return manager_->proxys_[id_];
    }
    ScriptProxy* ScriptProxyManager::Iterator::operator->() {
        return &(manager_->proxys_[id_]);
    }

    ScriptProxyManager::Iterator::Iterator(ScriptProxyManager* manager, U32 id) : manager_(manager), id_(id) {}

    ScriptProxyManager::Iterator ScriptProxyManager::begin() {
        U32 id = 0;
        while (id < states_.size() && !states_[id]) {
            ++id;
        }
        return Iterator(this, id);
    }
    ScriptProxyManager::Iterator ScriptProxyManager::end() {
        return Iterator(this, states_.size());
    }

    void ScriptProxyManager::Clear() {
        states_.clear();
        proxys_.clear();
        states_.clear();
    }
    void ScriptProxyManager::Destroy(U32 id) {
        available_ids_.push_back(id);
        states_[id] = false;
    }
    void ScriptProxyManager::Destroy(Iterator it) {
        Destroy(it.id_);
    }

    ScriptProxy& ScriptProxyManager::GetProxy(U32 id) {
        return proxys_[id];
    }
    U32 ScriptProxyManager::CreateProxy() {
        ExtendIfRequired();
        U32 id = available_ids_.front();
        available_ids_.pop_front();
        states_[id]          = true;
        proxys_[id].ScriptID = UUID();
        proxys_[id].Script   = nullptr;
        return id;
    }

    void ScriptProxyManager::ExtendIfRequired() {
        if (available_ids_.empty()) {
            available_ids_.push_back(proxys_.size());
            proxys_.push_back({nullptr, 0, UUID()});
            states_.push_back(false);
        }
    }

    S_INITIALIZE() {
        LOG_INFO("ScriptEngine initialized");
        return Unit();
    }
    S_TERMINATE() {
        ClearLibraries();
        LOG_INFO("ScriptEngine terminated");
        return Unit();
    }

    S_METHOD_IMPL(bool, AddScript, (UUID id), (id)) {
        auto asset = AssetManager::GetScriptAsset(id);
        if (!asset) {
            LOG_WARNING("ScriptAsset {} was not found in AssetManager", id);
            return false;
        }
        const auto& name = asset.value().Name;

        if (script_classes_.contains(id)) {
            LOG_WARNING("Script {} wasn't added because already exists", id);
            return false;
        }
        script_classes_[id] = ScriptClass(name);
        for (auto lib : libraries_) {
            script_classes_[id].Load(lib);
            if (script_classes_[id].Valid()) {
                LOG_INFO("Script ({}|{}) found in library {}", name, id, lib->GetName());
                for (auto& instance : proxy_manager_) {
                    if (instance.ScriptID == id) {
                        instance.Script = script_classes_[id].Create();
                        LOG_INFO("Created instance of ({}|{})", name, id);
                    }
                }
                return true;
            }
        }
        LOG_WARNING("Script ({}|{}) was not found in any library", name, id);
        return false;
    }
    S_METHOD_IMPL(bool, DeleteScript, (UUID id), (id)) {
        if (!script_classes_.contains(id)) {
            LOG_WARNING("Script {} was not deleted because does not exists", id);
            return false;
        }
        if (script_classes_[id].Valid()) {
            for (auto& instance : proxy_manager_) {
                if (instance.ScriptID == id) {
                    script_classes_[id].Delete(instance.Script);
                    instance.Script = nullptr;
                    LOG_INFO("Deleted instance of ({}|{})", script_classes_[id].GetName(), id);
                }
            }
        }
        script_classes_.erase(id);
        LOG_INFO("Script {} was deleted", id);
        return true;
    }
    S_METHOD_IMPL(bool, ValidScript, (UUID id), (id)) {
        return script_classes_.contains(id) && script_classes_[id].Valid();
    }
    S_METHOD_IMPL(Unit, ClearScripts, (), ()) {
        for (auto& instance : proxy_manager_) {
            if (instance.Script != nullptr) {
                DE_ASSERT(script_classes_.contains(instance.ScriptID), "Internal state broken");
                script_classes_[instance.ScriptID].Delete(instance.Script);
                instance.Script = nullptr;
                LOG_INFO("Deleted instance of ({}|{})", script_classes_[instance.ScriptID].GetName(), instance.ScriptID);
            }
        }
        script_classes_.clear();
        LOG_INFO("Cleared all scripts");
        return Unit();
    }

    S_METHOD_IMPL(Unit, AddLibrary, (Ref<SharedObject> library), (library)) {
        for (size_t i = 0; i < libraries_.size(); ++i) {
            if (libraries_[i]->GetName() == library->GetName()) {
                DeleteLibrary(libraries_[i]->GetName());
                LOG_WARNING("Deleted library {} because already loaded", library->GetName());
                break;
            }
        }
        libraries_.push_back(library);
        UpdateScriptClasses(library);
        LOG_INFO("Library {} was added", library->GetName());
        return Unit();
    }
    S_METHOD_IMPL(Unit, DeleteLibrary, (const std::string& name), (name)) {
        S32 id = -1;
        //*Find library
        {
            for (size_t i = 0; i < libraries_.size(); ++i) {
                if (libraries_[i]->GetName() == name) {
                    id = i;
                    break;
                }
            }
            if (id == -1) {
                LOG_WARNING("Library {} was not deleted because does not exist", name);
                return Unit();
            }
        }

        libraries_[id]->Invalidate();

        //*Invalidate Instances
        {
            for (auto& instance : proxy_manager_) {
                if (script_classes_.contains(instance.ScriptID) && !script_classes_[instance.ScriptID].Valid()) {
                    if (instance.Script != nullptr) {
                        script_classes_[instance.ScriptID].Delete(instance.Script);
                        instance.Script = nullptr;
                        LOG_INFO("Deleted instance of ({}|{})", script_classes_[instance.ScriptID].GetName(), instance.ScriptID);
                    }
                }
            }
        }

        libraries_.erase(libraries_.begin() + id);
        LOG_INFO("Library {} was deleted", name);
        return Unit();
    }
    S_METHOD_IMPL(bool, LibraryLoaded, (const std::string& name), (name)) {
        for (auto lib : libraries_) {
            if (lib->GetName() == name) {
                return true;
            }
        }
        return false;
    }
    S_METHOD_IMPL(Unit, ClearLibraries, (), ()) {
        for (auto lib : libraries_) {
            lib->Invalidate();
        }
        for (auto& instance : proxy_manager_) {
            if (instance.Script != nullptr) {
                DE_ASSERT(script_classes_.contains(instance.ScriptID), "Internal state broken");
                script_classes_[instance.ScriptID].Delete(instance.Script);
                instance.Script = nullptr;
                LOG_INFO("Deleted instance of ({}|{})", script_classes_[instance.ScriptID].GetName(), instance.ScriptID);
            }
        }
        libraries_.clear();
        LOG_INFO("Cleared all libraries");
        return Unit();
    }

    S_METHOD_IMPL(ScriptComponent, CreateScript, (UUID id), (id)) {
        auto            p_id  = proxy_manager_.CreateProxy();
        auto&           proxy = proxy_manager_.GetProxy(p_id);
        ScriptComponent res(p_id);
        proxy.ScriptID = id;
        if (!script_classes_.contains(id)) {
            LOG_WARNING("Creating ScriptComponent of unknown ScriptClass {}", id);
        } else {
            if (script_classes_[id].Valid()) {
                proxy.Script = script_classes_[id].Create();
                LOG_INFO("Created instance of ({}|{})", script_classes_[id].GetName(), id);
            } else {
                LOG_WARNING("Creating ScriptComponent of not yet loaded ScriptClass {}", id);
            }
        }
        return res;
    }
    S_METHOD_IMPL(Unit, IncreaseRefCount, (U32 id), (id)) {
        auto& proxy = proxy_manager_.GetProxy(id);
        ++proxy.RefCount;
        return Unit();
    }
    S_METHOD_IMPL(Unit, DecreaseRefCount, (U32 id), (id)) {
        auto& proxy = proxy_manager_.GetProxy(id);
        DE_ASSERT(proxy.RefCount != 0, "Ref Counter error");
        --proxy.RefCount;
        if (proxy.RefCount == 0) {
            Destroy(id);
        }
        return Unit();
    }
    void ScriptEngine::Destroy(U32 id) {
        ScriptProxy& proxy = proxy_manager_.GetProxy(id);
        if (proxy.Script != nullptr) {
            DE_ASSERT(script_classes_.contains(proxy.ScriptID), "Internal state broken");
            script_classes_[proxy.ScriptID].Delete(proxy.Script);
            proxy.Script = nullptr;
            LOG_INFO("Deleted instance of ({}|{})", script_classes_[proxy.ScriptID].GetName(), proxy.ScriptID);
        }
        proxy_manager_.Destroy(id);
    }

    void ScriptEngine::UpdateScriptClasses(Ref<SharedObject> library) {
        for (auto& [id, script_class] : script_classes_) {
            if (!script_class.Valid()) {
                if (script_class.Load(library)) {
                    LOG_INFO("ScriptClass {} was  found in library {}", script_class.GetName(), library->GetName());
                }
            }
        }
        for (auto& instance : proxy_manager_) {
            if (instance.Script == nullptr && script_classes_.contains(instance.ScriptID) && script_classes_[instance.ScriptID].Valid()) {
                instance.Script = script_classes_[instance.ScriptID].Create();
                LOG_INFO("Created instance of ({}|{})", script_classes_[instance.ScriptID].GetName(), instance.ScriptID);
            }
        }
    }

}  // namespace DummyEngine