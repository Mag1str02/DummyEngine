#include "DummyEngine/Core/Scripting/ScriptEngine.h"

namespace DE
{
    Path PathToCompiledScript(Path path)
    {
        return Config::GetPath(DE_CFG_SCRIPT_CACHE_PATH) / (fs::relative(path, Config::GetPath(DE_CFG_SCRIPT_PATH)).string() + ".o");
    }

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
    Unit ScriptEngine::ITerminate() { return Unit(); }

    S_METHOD_IMPL(ScriptEngine, Unit, AddScript, (const ScriptAsset& asset), (asset))
    {
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
        m_HandleManager.Clear();
        m_ScriptClasses.clear();
        return Unit();
    }

    S_METHOD_IMPL(ScriptEngine, Unit, AddLibrary, (Ref<SharedObject> library), (library))
    {
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
        if (!Valid(id))
        {
            return Handle<ScriptInstance>();
        }
        auto res           = m_HandleManager.CreateHandle();
        res.Get().m_Script = m_ScriptClasses[id].CreateInstance();
        res.Get().m_Id     = id;
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

    //*

    // S_METHOD_IMPL(ScriptEngine, Unit, DeleteScript, (UUID id), (id))
    // {
    //     m_ScriptStates.erase(id);
    //     m_CreateFuncs.erase(id);
    //     return Unit();
    // }
    // S_METHOD_IMPL(ScriptEngine, Unit, Modify, (UUID id), (id))
    // {
    //     if (m_ScriptStates.contains(id))
    //     {
    //         m_ScriptStates[id].modified = true;
    //     }
    //     return Unit();
    // }
    // S_METHOD_IMPL(ScriptEngine, Unit, Clear, (), ())
    // {
    //     m_CreateFuncs.clear();
    //     m_ScriptStates.clear();
    //     m_ScriptLibrary = nullptr;
    //     return Unit();
    // }

    // S_METHOD_IMPL(ScriptEngine, bool, ReloadSripts, (), ())
    // {
    //     if (m_ScriptStates.empty())
    //     {
    //         return false;
    //     }
    //     std::vector<Path> objects_list;
    //     if (!SourcesExist() || !UnModifiedObjectsExist())
    //     {
    //         return false;
    //     }

    //     for (const auto& [id, state] : m_ScriptStates)
    //     {
    //         objects_list.push_back(PathToCompiledScript(state.path));
    //         if (state.modified)
    //         {
    //             if (!Compiler::Compile(state.path, objects_list.back()))
    //             {
    //                 return false;
    //             }
    //         }
    //     }
    //     std::string library_name = "ScriptsLibrary";
    //     m_ScriptLibrary          = CreateScope<SharedObject>();
    //     if (!Compiler::Link(objects_list, Config::GetPath(DE_CFG_SCRIPT_CACHE_PATH), library_name))
    //     {
    //         return false;
    //     }
    //     if (!m_ScriptLibrary->Load(Config::GetPath(DE_CFG_SCRIPT_CACHE_PATH), library_name))
    //     {
    //         return false;
    //     }
    //     bool res = UpdateFuncTable();
    //     return res;
    // }
    // S_METHOD_IMPL(ScriptEngine, bool, Valid, (UUID id), (id)) { return m_CreateFuncs.contains(id); }
    // S_METHOD_IMPL(ScriptEngine, bool, AddScript, (const ScriptAsset& asset), (asset))
    // {
    //     if (!FileSystem::IsSubPath(asset.path, Config::GetPath(DE_CFG_SCRIPT_PATH)))
    //     {
    //         return false;
    //     }
    //     if (m_ScriptStates.contains(asset.id))
    //     {
    //         return false;
    //     }
    //     m_ScriptStates[asset.id].modified = true;
    //     m_ScriptStates[asset.id].path     = asset.path;
    //     m_CreateFuncs[asset.id]           = nullptr;

    //     return true;
    // }

    // S_METHOD_IMPL(ScriptEngine, Ref<Script>, CreateScript, (UUID id), (id))
    // {
    //     if (!m_CreateFuncs.contains(id))
    //     {
    //         return nullptr;
    //     }
    //     return m_CreateFuncs[id]();
    // }

    // bool ScriptEngine::SourcesExist() const
    // {
    //     for (const auto& [id, state] : m_ScriptStates)
    //     {
    //         if (!fs::exists(state.path))
    //         {
    //             return false;
    //         }
    //     }
    //     return true;
    // }
    // bool ScriptEngine::UnModifiedObjectsExist() const
    // {
    //     for (const auto& [id, state] : m_ScriptStates)
    //     {
    //         if (state.modified)
    //         {
    //             continue;
    //         }
    //         if (!fs::exists(PathToCompiledScript(state.path)))
    //         {
    //             return false;
    //         }
    //     }
    //     return true;
    // }
    // bool ScriptEngine::UpdateFuncTable()
    // {
    //     std::unordered_map<UUID, CreateScriptFunc> res;
    //     for (const auto& [id, state] : m_ScriptStates)
    //     {
    //         CreateScriptFunc create_func = (CreateScriptFunc)m_ScriptLibrary->GetFunction("CreateInstance" + state.path.stem().string());
    //         if (!create_func)
    //         {
    //             return false;
    //         }
    //         res[id] = create_func;
    //         ScriptClass s_class(state.path.stem().string());
    //         s_class.Load(m_ScriptLibrary);
    //         LOG_INFO(StrCat("Class ", state.path.stem().string(), " ", (s_class.Valid() ? "Valid" : "Invalid")), "ScriptEngine");
    //     }
    //     m_CreateFuncs = std::move(res);
    //     return true;
    // }

}  // namespace DE