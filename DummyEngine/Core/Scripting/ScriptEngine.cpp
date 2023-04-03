#include "DummyEngine/Core/Scripting/ScriptEngine.h"

#include "DummyEngine/Core/ResourceManaging/AssetManager.h"
#include "DummyEngine/Core/Scene/Components.h"
#include "DummyEngine/Core/Scene/Scene.h"

namespace DE {
    UUID ScriptComponent::ID() const {
        return ScriptEngine::GetUUID(*this);
    }
    bool ScriptComponent::Valid() const {
        return ScriptEngine::Valid(*this);
    }
    bool ScriptComponent::Loaded() const {
        return ScriptEngine::Loaded(*this);
    }
    void ScriptComponent::Destroy() {
        ScriptEngine::Destroy(*this);
    }
    Script& ScriptComponent::operator*() {
        return *ScriptEngine::GetScript(*this);
    }
    Script* ScriptComponent::operator->() {
        return ScriptEngine::GetScript(*this);
    }

    SINGLETON_BASE(ScriptEngine);
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
                    if (instance.m_ID == id) {
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
                if (instance.m_ID == id) {
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
                m_ScriptClasses[instance.m_ID].Delete(instance.m_Script);
                instance.m_Script = nullptr;
                LOG_INFO("ScriptEngine", "Deleted instance of (", m_ScriptClasses[instance.m_ID].GetName(), "|", instance.m_ID, ")");
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
                if (!m_ScriptClasses[instance.m_ID].Valid()) {
                    if (instance.m_Script) {
                        m_ScriptClasses[instance.m_ID].Delete(instance.m_Script);
                        instance.m_Script = nullptr;
                        LOG_INFO("ScriptEngine", "Deleted instance of (", m_ScriptClasses[instance.m_ID].GetName(), "|", instance.m_ID, ")");
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
                m_ScriptClasses[instance.m_ID].Delete(instance.m_Script);
                instance.m_Script = nullptr;
                LOG_INFO("ScriptEngine", "Deleted instance of (", m_ScriptClasses[instance.m_ID].GetName(), "|", instance.m_ID, ")");
            }
        }
        m_Libraries.clear();
        LOG_INFO("ScriptEngine", "Cleared all libraries");
        return Unit();
    }

    S_METHOD_IMPL(ScriptComponent, CreateScript, (UUID id), (id)) {
        auto [p_id, gen]      = m_ProxyManager.CreateProxy();
        auto&           proxy = m_ProxyManager.GetProxy(p_id);
        ScriptComponent res(p_id, gen);
        proxy.m_ID = id;
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
    S_METHOD_IMPL(Script*, GetScript, (const ScriptComponent& component), (component)) {
        return m_ProxyManager.GetProxy(component.m_ID).m_Script;
    }
    S_METHOD_IMPL(UUID, GetUUID, (const ScriptComponent& component), (component)) {
        return m_ProxyManager.GetProxy(component.m_ID).m_ID;
    }
    S_METHOD_IMPL(bool, Valid, (const ScriptComponent& component), (component)) {
        return m_ProxyManager.Valid(component.m_ID, component.m_Gen);
    }
    S_METHOD_IMPL(bool, Loaded, (const ScriptComponent& component), (component)) {
        return m_ProxyManager.Valid(component.m_ID, component.m_Gen) && m_ProxyManager.GetProxy(component.m_ID).m_Script;
    }
    S_METHOD_IMPL(Unit, Destroy, (const ScriptComponent& component), (component)) {
        if (m_ProxyManager.Valid(component.m_ID, component.m_Gen)) {
            ScriptProxy& proxy = m_ProxyManager.GetProxy(component.m_ID);
            if (proxy.m_Script) {
                m_ScriptClasses[proxy.m_ID].Delete(proxy.m_Script);
                proxy.m_Script = nullptr;
                LOG_INFO("ScriptEngine", "Deleted instance of (", m_ScriptClasses[proxy.m_ID].GetName(), "|", proxy.m_ID, ")");
            }
            m_ProxyManager.Destroy(component.m_ID);
        }
        return Unit();
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
            if (!instance.m_Script && m_ScriptClasses.contains(instance.m_ID) && m_ScriptClasses[instance.m_ID].Valid()) {
                instance.m_Script = m_ScriptClasses[instance.m_ID].Create();
                LOG_INFO("ScriptEngine", "Created instance of (", m_ScriptClasses[instance.m_ID].GetName(), "|", instance.m_ID, ")");
            }
        }
    }

}  // namespace DE