#include "DummyEngine/Core/Scripting/ScriptEngine.h"
#include "DummyEngine/Core/Scene/Scene.h"
#include "DummyEngine/Core/Scene/Components.h"

namespace DE
{
    UUID    ScriptComponent::ID() const { return ScriptEngine::GetUUID(*this); }
    bool    ScriptComponent::Valid() const { return ScriptEngine::Valid(*this); }
    void    ScriptComponent::Destroy() { ScriptEngine::Destroy(*this); }
    Script& ScriptComponent::operator*() { return *ScriptEngine::GetScript(*this); }
    Script* ScriptComponent::operator->() { return ScriptEngine::GetScript(*this); }

    SINGLETON_BASE(ScriptEngine);

    Unit ScriptEngine::Initialize()
    {
        DE_ASSERT(!s_Instance, "Double ScriptEngine initialization");
        s_Instance = new ScriptEngine();
        DE_ASSERT(s_Instance, "Failed to allocate memory for ScriptEngine");
        s_Instance->IInitialize();
        return Unit();
    }
    Unit ScriptEngine::IInitialize()
    {
        LOG_INFO("ScriptEngine", "ScriptEngine initialized");
        return Unit();
    }
    Unit ScriptEngine::Terminate()
    {
        s_Instance->ITerminate();
        delete s_Instance;
        return Unit();
    }
    Unit ScriptEngine::ITerminate()
    {
        ClearLibraries();
        LOG_INFO("ScriptEngine", "ScriptEngine terminated");
        return Unit();
    }

    S_METHOD_IMPL(ScriptEngine, Unit, AddScript, (const ScriptAsset& asset), (asset))
    {
        if (m_ScriptClasses.contains(asset.id))
        {
            LOG_WARNING("ScriptEngine", "Script (", asset.id.Hex(), "|", asset.name, ") wasn't added because already exists");
            return Unit();
        }
        m_ScriptClasses[asset.id] = ScriptClass(asset.name);
        for (auto lib : m_Libraries)
        {
            m_ScriptClasses[asset.id].Load(lib);
            if (m_ScriptClasses[asset.id].Valid())
            {
                LOG_INFO("ScriptEngine", "Script (", asset.id.Hex(), "|", asset.name, ") found in library (", lib->GetName(), ")");
                for (auto& instance : m_ProxyManager)
                {
                    if (instance.m_Id == asset.id)
                    {
                        instance.m_Script = m_ScriptClasses[asset.id].Create();
                        LOG_INFO("ScriptEngine", "Created instance of (", asset.id.Hex(), "|", asset.name, ")");
                    }
                }
                return Unit();
            }
        }
        LOG_WARNING("ScriptEngine", "Script (", asset.id.Hex(), "|", asset.name, ") was not found in any library");
        return Unit();
    }
    S_METHOD_IMPL(ScriptEngine, Unit, DeleteScript, (UUID id), (id))
    {
        if (!m_ScriptClasses.contains(id))
        {
            LOG_WARNING("ScriptEngine", "Script (", id.Hex(), ") was not deleted because does not exists");
            return Unit();
        }
        if (m_ScriptClasses[id].Valid())
        {
            for (auto& instance : m_ProxyManager)
            {
                if (instance.m_Id == id)
                {
                    m_ScriptClasses[id].Delete(instance.m_Script);
                    instance.m_Script = nullptr;
                    LOG_INFO("ScriptEngine", "Deleted instance of (", id.Hex(), ")");
                }
            }
        }
        m_ScriptClasses.erase(id);
        LOG_INFO("ScriptEngine", "Script (", id.Hex(), ") was deleted");
        return Unit();
    }
    S_METHOD_IMPL(ScriptEngine, bool, Valid, (UUID id), (id)) { return m_ScriptClasses.contains(id) && m_ScriptClasses[id].Valid(); }
    S_METHOD_IMPL(ScriptEngine, Unit, ClearScripts, (), ())
    {
        for (auto& instance : m_ProxyManager)
        {
            if (instance.m_Script)
            {
                m_ScriptClasses[instance.m_Id].Delete(instance.m_Script);
                instance.m_Script = nullptr;
                LOG_INFO("ScriptEngine", "Deleted instance of (", instance.m_Id.Hex(), ")");
            }
        }
        m_ScriptClasses.clear();
        LOG_INFO("ScriptEngine", "Cleared all scripts");
        return Unit();
    }

    S_METHOD_IMPL(ScriptEngine, Unit, AddLibrary, (Ref<SharedObject> library), (library))
    {
        for (size_t i = 0; i < m_Libraries.size(); ++i)
        {
            if (m_Libraries[i]->GetName() == library->GetName())
            {
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
    S_METHOD_IMPL(ScriptEngine, Unit, DeleteLibrary, (const std::string& name), (name))
    {
        int32_t id = -1;
        //*Find library
        {
            for (size_t i = 0; i < m_Libraries.size(); ++i)
            {
                if (m_Libraries[i]->GetName() == name)
                {
                    id = i;
                    break;
                }
            }
            if (id == -1)
            {
                LOG_WARNING("ScriptEngine", "Library (", name, ") was not deleted because does not exist");
                return Unit();
            }
        }

        m_Libraries[id]->Invalidate();

        //*Invalidate Instances
        {
            for (auto& instance : m_ProxyManager)
            {
                if (!m_ScriptClasses[instance.m_Id].Valid())
                {
                    if (instance.m_Script)
                    {
                        m_ScriptClasses[instance.m_Id].Delete(instance.m_Script);
                        instance.m_Script = nullptr;
                        LOG_INFO("ScriptEngine", "Deleted instance of (", instance.m_Id.Hex(), ")");
                    }
                }
            }
        }

        m_Libraries.erase(m_Libraries.begin() + id);
        LOG_INFO("ScriptEngine", "Library (", name, ") was deleted");
        return Unit();
    }
    S_METHOD_IMPL(ScriptEngine, bool, LibraryLoaded, (const std::string& name), (name))
    {
        for (auto lib : m_Libraries)
        {
            if (lib->GetName() == name)
            {
                return true;
            }
        }
        return false;
    }
    S_METHOD_IMPL(ScriptEngine, Unit, ClearLibraries, (), ())
    {
        for (auto lib : m_Libraries)
        {
            lib->Invalidate();
        }
        for (auto& instance : m_ProxyManager)
        {
            if (instance.m_Script)
            {
                m_ScriptClasses[instance.m_Id].Delete(instance.m_Script);
                instance.m_Script = nullptr;
                LOG_INFO("ScriptEngine", "Deleted instance of (", instance.m_Id.Hex(), ")");
            }
        }
        m_Libraries.clear();
        LOG_INFO("ScriptEngine", "Cleared all libraries");
        return Unit();
    }

    S_METHOD_IMPL(ScriptEngine, ScriptComponent, CreateScript, (UUID id), (id))
    {
        auto [p_id, gen]      = m_ProxyManager.CreateProxy();
        auto&           proxy = m_ProxyManager.GetProxy(p_id);
        ScriptComponent res(p_id, gen);
        proxy.m_Id = id;
        if (!m_ScriptClasses.contains(id))
        {
            LOG_WARNING("ScriptEngine", "Creating ScriptComponent of unknown ScriptClass (", id.Hex(), ")");
        }
        else
        {
            if (m_ScriptClasses[id].Valid())
            {
                proxy.m_Script = m_ScriptClasses[id].Create();
                LOG_INFO("ScriptEngine", "Created instance of (", id.Hex(), ")");
            }
            else
            {
                LOG_WARNING("ScriptEngine", "Creating ScriptComponent of not yet loaded ScriptClass (", id.Hex(), ")");
            }
        }
        return res;
    }
    S_METHOD_IMPL(ScriptEngine, Script*, GetScript, (const ScriptComponent& component), (component))
    {
        return m_ProxyManager.GetProxy(component.m_ID).m_Script;
    }
    S_METHOD_IMPL(ScriptEngine, UUID, GetUUID, (const ScriptComponent& component), (component))
    {
        return m_ProxyManager.GetProxy(component.m_ID).m_Id;
    }
    S_METHOD_IMPL(ScriptEngine, bool, Valid, (const ScriptComponent& component), (component))
    {
        return m_ProxyManager.Valid(component.m_ID, component.m_Gen) && m_ProxyManager.GetProxy(component.m_ID).m_Script;
    }
    S_METHOD_IMPL(ScriptEngine, Unit, Destroy, (const ScriptComponent& component), (component))
    {
        if (m_ProxyManager.Valid(component.m_ID, component.m_Gen))
        {
            ScriptProxy& proxy = m_ProxyManager.GetProxy(component.m_ID);
            if (proxy.m_Script)
            {
                m_ScriptClasses[proxy.m_Id].Delete(proxy.m_Script);
                proxy.m_Script = nullptr;
                LOG_INFO("ScriptEngine", "Deleted instance of (", proxy.m_Id.Hex(), ")");
            }
            m_ProxyManager.Destroy(component.m_ID);
        }
        return Unit();
    }

    void ScriptEngine::UpdateScriptClasses(Ref<SharedObject> library)
    {
        for (auto& [id, script_class] : m_ScriptClasses)
        {
            if (!script_class.Valid())
            {
                if (script_class.Load(library))
                {
                    LOG_INFO("ScriptEngine", "ScriptClass (", script_class.GetName(), ") was  found in library (", library->GetName(), ")");
                }
            }
        }
        for (auto& instance : m_ProxyManager)
        {
            if (!instance.m_Script && m_ScriptClasses.contains(instance.m_Id) && m_ScriptClasses[instance.m_Id].Valid())
            {
                instance.m_Script = m_ScriptClasses[instance.m_Id].Create();
                LOG_INFO("ScriptEngine", "Created instance of (", instance.m_Id.Hex(), ")");
            }
        }
    }

}  // namespace DE