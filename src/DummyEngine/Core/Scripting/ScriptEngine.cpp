#include "DummyEngine/Core/Scripting/ScriptEngine.h"

namespace DE
{
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
        for (auto it = m_HandleManager.begin(); it != m_HandleManager.end(); ++it)
        {
            if (it->m_Id == id)
            {
                //! Abuses iterator invalidation
                m_HandleManager.Destroy(it.ID());
            }
        }
        m_ScriptClasses.erase(id);
        return Unit();
    }
    S_METHOD_IMPL(ScriptEngine, bool, Valid, (UUID id), (id)) { return m_ScriptClasses.contains(id) && m_ScriptClasses[id].Valid(); }
    S_METHOD_IMPL(ScriptEngine, Unit, ClearScripts, (), ())
    {
        LOG_INFO("Clearing scripts...", "ScriptEngine");
        m_HandleManager.Clear();
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
            std::unordered_set<UUID> invalid_classes = GetInvalidClasses();
            for (auto& instance : m_HandleManager)
            {
                if (invalid_classes.contains(instance.m_Id))
                {
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
        for (auto& script_instance : m_HandleManager)
        {
            script_instance.m_Script = nullptr;
        }
        m_Libraries.clear();
        return Unit();
    }

    S_METHOD_IMPL(ScriptEngine, Handle<ScriptInstance>, CreateScript, (UUID id), (id))
    {
        if (!m_ScriptClasses.contains(id))
        {
            LOG_INFO(StrCat("Creating unknown script: ", std::to_string(id)), "ScriptEngine");
            return Handle<ScriptInstance>();
        }
        auto res        = m_HandleManager.CreateHandle();
        (*res).m_Script = m_ScriptClasses[id].CreateInstance();
        (*res).m_Id     = id;
        LOG_INFO(StrCat("Creating script: ", std::to_string(id), " Handle ID: ", std::to_string(res.GetId())), "ScriptEngine");
        return res;
    }

    void ScriptEngine::UpdateScriptClasses(Ref<SharedObject> library)
    {
        std::unordered_set<UUID> loaded_classes;
        std::unordered_set<UUID> invalid_classes = GetInvalidClasses();

        for (auto id : invalid_classes)
        {
            if (m_ScriptClasses[id].Load(library))
            {
                loaded_classes.insert(id);
            }
        }
        for (auto& instance : m_HandleManager)
        {
            if (loaded_classes.contains(instance.m_Id))
            {
                instance.m_Script = m_ScriptClasses[instance.m_Id].CreateInstance();
            }
        }
    }
    std::unordered_set<UUID> ScriptEngine::GetInvalidClasses()
    {
        std::unordered_set<UUID> loaded_classes;
        for (auto& [id, script_class] : m_ScriptClasses)
        {
            if (!script_class.Valid())
            {
                loaded_classes.insert(id);
            }
        }
        return loaded_classes;
    }

}  // namespace DE