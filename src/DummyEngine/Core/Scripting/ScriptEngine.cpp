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
    Unit ScriptEngine::IInitialize() { return Unit(); }
    Unit ScriptEngine::Terminate()
    {
        s_Instance->ITerminate();
        delete s_Instance;
        return Unit();
    }
    Unit ScriptEngine::ITerminate()
    {
        ClearLibraries();
        return Unit();
    }

    S_METHOD_IMPL(ScriptEngine, Unit, AddScript, (const ScriptAsset& asset), (asset))
    {
        LOG_INFO(StrCat("Adding script: ", asset.name, " - ", std::to_string(asset.id)), "ScriptEngine");
        if (m_ScriptClasses.contains(asset.id))
        {
            return Unit();
        }
        m_ScriptClasses[asset.id] = ScriptClass(asset.name);
        for (auto lib : m_Libraries)
        {
            m_ScriptClasses[asset.id].Load(lib);
            if (m_ScriptClasses[asset.id].Valid())
            {
                for (auto& instance : m_ProxyManager)
                {
                    if (instance.m_Id == asset.id)
                    {
                        instance.m_Script = m_ScriptClasses[asset.id].Create();
                    }
                }
                break;
            }
        }
        return Unit();
    }
    S_METHOD_IMPL(ScriptEngine, Unit, DeleteScript, (UUID id), (id))
    {
        LOG_INFO(StrCat("Deleting script: ", std::to_string(id)), "ScriptEngine");
        if (!m_ScriptClasses.contains(id))
        {
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
                }
            }
        }
        m_ScriptClasses.erase(id);
        return Unit();
    }
    S_METHOD_IMPL(ScriptEngine, bool, Valid, (UUID id), (id)) { return m_ScriptClasses.contains(id) && m_ScriptClasses[id].Valid(); }
    S_METHOD_IMPL(ScriptEngine, Unit, ClearScripts, (), ())
    {
        LOG_INFO("Clearing scripts...", "ScriptEngine");
        for (auto& instance : m_ProxyManager)
        {
            if (instance.m_Script)
            {
                m_ScriptClasses[instance.m_Id].Delete(instance.m_Script);
            }
            instance.m_Script = nullptr;
        }
        m_ScriptClasses.clear();
        return Unit();
    }

    S_METHOD_IMPL(ScriptEngine, Unit, AddLibrary, (Ref<SharedObject> library), (library))
    {
        LOG_INFO(StrCat("Adding library: ", library->GetName()), "ScriptEngine");
        for (size_t i = 0; i < m_Libraries.size(); ++i)
        {
            if (m_Libraries[i]->GetName() == library->GetName())
            {
                DeleteLibrary(m_Libraries[i]->GetName());
                break;
            }
        }
        m_Libraries.push_back(library);
        UpdateScriptClasses(library);
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
                    }
                    instance.m_Script = nullptr;
                }
            }
        }

        m_Libraries.erase(m_Libraries.begin() + id);
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
            }
            instance.m_Script = nullptr;
        }
        m_Libraries.clear();
        return Unit();
    }

    S_METHOD_IMPL(ScriptEngine, ScriptComponent, CreateScript, (UUID id), (id))
    {
        LOG_INFO(StrCat("Creating script instance: ", std::to_string(id)), "ScriptEngine");
        auto [p_id, gen]      = m_ProxyManager.CreateProxy();
        auto&           proxy = m_ProxyManager.GetProxy(p_id);
        ScriptComponent res(p_id, gen);
        proxy.m_Id = id;
        if (!m_ScriptClasses.contains(id))
        {
            LOG_INFO(StrCat("Creating unknown script: ", std::to_string(id)), "ScriptEngine");
        }
        else
        {
            LOG_INFO(StrCat("Creating script from dll function..."), "ScriptEngine");
            if (m_ScriptClasses[id].Valid())
            {
                proxy.m_Script = m_ScriptClasses[id].Create();
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
                script_class.Load(library);
            }
        }
        for (auto& instance : m_ProxyManager)
        {
            if (!instance.m_Script)
            {
                instance.m_Script = m_ScriptClasses[instance.m_Id].Create();
            }
        }
    }

}  // namespace DE