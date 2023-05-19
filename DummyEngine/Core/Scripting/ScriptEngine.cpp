#include "DummyEngine/Core/Scripting/ScriptEngine.h"

#include "DummyEngine/Core/ResourceManaging/AssetManager.h"
#include "DummyEngine/Core/Scene/Components.h"
#include "DummyEngine/Core/Scene/Scene.h"

namespace DE {
    SINGLETON_BASE(ScriptEngine);

    ScriptComponent::ScriptComponent(U32 id) : m_ID(id) {
        ScriptEngine::IncreaseRefCount(m_ID);
    }
    ScriptComponent::ScriptComponent(const ScriptComponent& other) {
        m_ID = other.m_ID;
        if (other.m_ID != UINT32_MAX) {
            ScriptEngine::IncreaseRefCount(m_ID);
        }
    }
    ScriptComponent::ScriptComponent(ScriptComponent&& other) {
        m_ID       = other.m_ID;
        other.m_ID = UINT32_MAX;
    }
    ScriptComponent& ScriptComponent::operator=(const ScriptComponent& other) {
        if (&other != this) {
            if (m_ID != UINT32_MAX) {
                ScriptEngine::DecreaseRefCount(m_ID);
            }
            m_ID = other.m_ID;
            if (other.m_ID != UINT32_MAX) {
                ScriptEngine::IncreaseRefCount(m_ID);
            }
        }
        return *this;
    }
    ScriptComponent& ScriptComponent::operator=(ScriptComponent&& other) {
        if (&other != this) {
            if (m_ID != UINT32_MAX) {
                ScriptEngine::DecreaseRefCount(m_ID);
            }
            m_ID       = other.m_ID;
            other.m_ID = UINT32_MAX;
        }
        return *this;
    }
    ScriptComponent::~ScriptComponent() {
        if (m_ID != UINT32_MAX) {
            ScriptEngine::DecreaseRefCount(m_ID);
        }
    }
    UUID ScriptComponent::ID() const {
        return ScriptEngine::Get().m_ProxyManager.GetProxy(m_ID).m_ScriptID;
    }
    bool ScriptComponent::Valid() const {
        return m_ID != UINT32_MAX && ScriptEngine::Get().m_ProxyManager.GetProxy(m_ID).m_Script;
    }
    Script& ScriptComponent::operator*() {
        return *ScriptEngine::Get().m_ProxyManager.GetProxy(m_ID).m_Script;
    }
    Script* ScriptComponent::operator->() {
        return ScriptEngine::Get().m_ProxyManager.GetProxy(m_ID).m_Script;
    }

    bool ScriptProxyManager::Iterator::operator==(const Iterator& other) const {
        return m_Manager == other.m_Manager && m_ID == other.m_ID;
    }
    bool ScriptProxyManager::Iterator::operator!=(const Iterator& other) const {
        return m_ID != other.m_ID || m_Manager != other.m_Manager;
    }

    ScriptProxyManager::Iterator& ScriptProxyManager::Iterator::operator++() {
        do {
            ++m_ID;
        } while (m_ID < m_Manager->m_States.size() && !m_Manager->m_States[m_ID]);
        return *this;
    }
    ScriptProxyManager::Iterator ScriptProxyManager::Iterator::operator++(int) {
        Iterator res = *this;
        return ++(*this);
    }
    ScriptProxy& ScriptProxyManager::Iterator::operator*() {
        return m_Manager->m_Proxys[m_ID];
    }
    ScriptProxy* ScriptProxyManager::Iterator::operator->() {
        return &(m_Manager->m_Proxys[m_ID]);
    }

    ScriptProxyManager::Iterator::Iterator(ScriptProxyManager* manager, U32 id) : m_Manager(manager), m_ID(id) {}

    ScriptProxyManager::Iterator ScriptProxyManager::begin() {
        int id = 0;
        while (id < m_States.size() && !m_States[id]) {
            ++id;
        }
        return Iterator(this, id);
    }
    ScriptProxyManager::Iterator ScriptProxyManager::end() {
        return Iterator(this, m_States.size());
    }

    void ScriptProxyManager::Clear() {
        m_States.clear();
        m_Proxys.clear();
        m_States.clear();
    }
    void ScriptProxyManager::Destroy(U32 id) {
        m_AvailableIds.push_back(id);
        m_States[id] = false;
        // LOG_INFO("ScriptProxyManager", "Destroyed handle (", id, ")");
    }
    void ScriptProxyManager::Destroy(Iterator it) {
        Destroy(it.m_ID);
    }

    ScriptProxy& ScriptProxyManager::GetProxy(U32 id) {
        return m_Proxys[id];
    }
    U32 ScriptProxyManager::CreateProxy() {
        ExtendIfRequired();
        U32 id = m_AvailableIds.front();
        m_AvailableIds.pop_front();
        m_States[id]            = true;
        m_Proxys[id].m_ScriptID = UUID();
        m_Proxys[id].m_Script   = nullptr;
        // LOG_INFO("ScriptProxyManager", "Created handle (", id, ")");
        return id;
    }

    void ScriptProxyManager::ExtendIfRequired() {
        if (m_AvailableIds.empty()) {
            m_AvailableIds.push_back(m_Proxys.size());
            m_Proxys.push_back({nullptr, 0, UUID()});
            m_States.push_back(false);
        }
    }

    S_INITIALIZE() {
        LOG_INFO("ScriptEngine", "ScriptEngine initialized");
        return Unit();
    }
    S_TERMINATE() {
        ClearLibraries();
        LOG_INFO("ScriptEngine", "ScriptEngine terminated");
        return Unit();
    }

    S_METHOD_IMPL(bool, AddScript, (UUID id), (id)) {
        auto asset = AssetManager::GetScriptAsset(id);
        if (!asset) {
            LOG_WARNING("ScriptEngine", "ScriptAsset (", id, ") was not found in AssetManager");
            return false;
        }
        const auto& name = asset.value().name;

        if (m_ScriptClasses.contains(id)) {
            LOG_WARNING("ScriptEngine", "Script (", name, "|", id, ") wasn't added because already exists");
            return false;
        }
        m_ScriptClasses[id] = ScriptClass(name);
        for (auto lib : m_Libraries) {
            m_ScriptClasses[id].Load(lib);
            if (m_ScriptClasses[id].Valid()) {
                LOG_INFO("ScriptEngine", "Script (", name, "|", id, ") found in library (", lib->GetName(), ")");
                for (auto& instance : m_ProxyManager) {
                    if (instance.m_ScriptID == id) {
                        instance.m_Script = m_ScriptClasses[id].Create();
                        LOG_INFO("ScriptEngine", "Created instance of (", name, "|", id, ")");
                    }
                }
                return true;
            }
        }
        LOG_WARNING("ScriptEngine", "Script (", name, "|", id, ") was not found in any library");
        return false;
    }
    S_METHOD_IMPL(bool, DeleteScript, (UUID id), (id)) {
        if (!m_ScriptClasses.contains(id)) {
            LOG_WARNING("ScriptEngine", "Script (", id, ") was not deleted because does not exists");
            return false;
        }
        if (m_ScriptClasses[id].Valid()) {
            for (auto& instance : m_ProxyManager) {
                if (instance.m_ScriptID == id) {
                    m_ScriptClasses[id].Delete(instance.m_Script);
                    instance.m_Script = nullptr;
                    LOG_INFO("ScriptEngine", "Deleted instance of (", m_ScriptClasses[id].GetName(), "|", id, ")");
                }
            }
        }
        m_ScriptClasses.erase(id);
        LOG_INFO("ScriptEngine", "Script (", id, ") was deleted");
        return true;
    }
    S_METHOD_IMPL(bool, ValidScript, (UUID id), (id)) {
        return m_ScriptClasses.contains(id) && m_ScriptClasses[id].Valid();
    }
    S_METHOD_IMPL(Unit, ClearScripts, (), ()) {
        for (auto& instance : m_ProxyManager) {
            if (instance.m_Script) {
                DE_ASSERT(m_ScriptClasses.contains(instance.m_ScriptID), "Internal state broken");
                m_ScriptClasses[instance.m_ScriptID].Delete(instance.m_Script);
                instance.m_Script = nullptr;
                LOG_INFO("ScriptEngine", "Deleted instance of (", m_ScriptClasses[instance.m_ScriptID].GetName(), "|", instance.m_ScriptID, ")");
            }
        }
        m_ScriptClasses.clear();
        LOG_INFO("ScriptEngine", "Cleared all scripts");
        return Unit();
    }

    S_METHOD_IMPL(Unit, AddLibrary, (Ref<SharedObject> library), (library)) {
        for (size_t i = 0; i < m_Libraries.size(); ++i) {
            if (m_Libraries[i]->GetName() == library->GetName()) {
                DeleteLibrary(m_Libraries[i]->GetName());
                LOG_WARNING("ScriptEngine", "Deleted library (", library->GetName(), ") because already loaded");
                break;
            }
        }
        m_Libraries.push_back(library);
        UpdateScriptClasses(library);
        LOG_INFO("ScriptEngine", "Library (", library->GetName(), ") was added");
        return Unit();
    }
    S_METHOD_IMPL(Unit, DeleteLibrary, (const std::string& name), (name)) {
        S32 id = -1;
        //*Find library
        {
            for (size_t i = 0; i < m_Libraries.size(); ++i) {
                if (m_Libraries[i]->GetName() == name) {
                    id = i;
                    break;
                }
            }
            if (id == -1) {
                LOG_WARNING("ScriptEngine", "Library (", name, ") was not deleted because does not exist");
                return Unit();
            }
        }

        m_Libraries[id]->Invalidate();

        //*Invalidate Instances
        {
            for (auto& instance : m_ProxyManager) {
                if (m_ScriptClasses.contains(instance.m_ScriptID) && !m_ScriptClasses[instance.m_ScriptID].Valid()) {
                    if (instance.m_Script) {
                        m_ScriptClasses[instance.m_ScriptID].Delete(instance.m_Script);
                        instance.m_Script = nullptr;
                        LOG_INFO(
                            "ScriptEngine", "Deleted instance of (", m_ScriptClasses[instance.m_ScriptID].GetName(), "|", instance.m_ScriptID, ")");
                    }
                }
            }
        }

        m_Libraries.erase(m_Libraries.begin() + id);
        LOG_INFO("ScriptEngine", "Library (", name, ") was deleted");
        return Unit();
    }
    S_METHOD_IMPL(bool, LibraryLoaded, (const std::string& name), (name)) {
        for (auto lib : m_Libraries) {
            if (lib->GetName() == name) {
                return true;
            }
        }
        return false;
    }
    S_METHOD_IMPL(Unit, ClearLibraries, (), ()) {
        for (auto lib : m_Libraries) {
            lib->Invalidate();
        }
        for (auto& instance : m_ProxyManager) {
            if (instance.m_Script) {
                DE_ASSERT(m_ScriptClasses.contains(instance.m_ScriptID), "Internal state broken");
                m_ScriptClasses[instance.m_ScriptID].Delete(instance.m_Script);
                instance.m_Script = nullptr;
                LOG_INFO("ScriptEngine", "Deleted instance of (", m_ScriptClasses[instance.m_ScriptID].GetName(), "|", instance.m_ScriptID, ")");
            }
        }
        m_Libraries.clear();
        LOG_INFO("ScriptEngine", "Cleared all libraries");
        return Unit();
    }

    S_METHOD_IMPL(ScriptComponent, CreateScript, (UUID id), (id)) {
        auto            p_id  = m_ProxyManager.CreateProxy();
        auto&           proxy = m_ProxyManager.GetProxy(p_id);
        ScriptComponent res(p_id);
        proxy.m_ScriptID = id;
        if (!m_ScriptClasses.contains(id)) {
            LOG_WARNING("ScriptEngine", "Creating ScriptComponent of unknown ScriptClass (", id, ")");
        } else {
            if (m_ScriptClasses[id].Valid()) {
                proxy.m_Script = m_ScriptClasses[id].Create();
                LOG_INFO("ScriptEngine", "Created instance of (", m_ScriptClasses[id].GetName(), "|", id, ")");
            } else {
                LOG_WARNING("ScriptEngine", "Creating ScriptComponent of not yet loaded ScriptClass (", id, ")");
            }
        }
        return res;
    }
    S_METHOD_IMPL(Unit, IncreaseRefCount, (U32 id), (id)) {
        auto& proxy = m_ProxyManager.GetProxy(id);
        ++proxy.m_RefCount;
        return Unit();
    }
    S_METHOD_IMPL(Unit, DecreaseRefCount, (U32 id), (id)) {
        auto& proxy = m_ProxyManager.GetProxy(id);
        DE_ASSERT(proxy.m_RefCount != 0, "Ref Counter error");
        --proxy.m_RefCount;
        if (proxy.m_RefCount == 0) {
            Destroy(id);
        }
        return Unit();
    }
    void ScriptEngine::Destroy(U32 id) {
        ScriptProxy& proxy = m_ProxyManager.GetProxy(id);
        if (proxy.m_Script) {
            DE_ASSERT(m_ScriptClasses.contains(proxy.m_ScriptID), "Internal state broken");
            m_ScriptClasses[proxy.m_ScriptID].Delete(proxy.m_Script);
            proxy.m_Script = nullptr;
            LOG_INFO("ScriptEngine", "Deleted instance of (", m_ScriptClasses[proxy.m_ScriptID].GetName(), "|", proxy.m_ScriptID, ")");
        }
        m_ProxyManager.Destroy(id);
    }

    void ScriptEngine::UpdateScriptClasses(Ref<SharedObject> library) {
        for (auto& [id, script_class] : m_ScriptClasses) {
            if (!script_class.Valid()) {
                if (script_class.Load(library)) {
                    LOG_INFO("ScriptEngine", "ScriptClass (", script_class.GetName(), ") was  found in library (", library->GetName(), ")");
                }
            }
        }
        for (auto& instance : m_ProxyManager) {
            if (!instance.m_Script && m_ScriptClasses.contains(instance.m_ScriptID) && m_ScriptClasses[instance.m_ScriptID].Valid()) {
                instance.m_Script = m_ScriptClasses[instance.m_ScriptID].Create();
                LOG_INFO("ScriptEngine", "Created instance of (", m_ScriptClasses[instance.m_ScriptID].GetName(), "|", instance.m_ScriptID, ")");
            }
        }
    }

}  // namespace DE