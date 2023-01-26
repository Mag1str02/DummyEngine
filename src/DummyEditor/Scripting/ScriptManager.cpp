#include "DummyEngine/Core/Scripting/ScriptEngine.h"
#include "DummyEngine/Core/Scripting/Compiler.h"

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
    Unit ScriptEngine::IInitialize()
    {
        m_ScriptLibrary = CreateRef<SharedObject>();
        return Unit();
    }
    Unit ScriptEngine::Terminate()
    {
        s_Instance->ITerminate();
        delete s_Instance;
        return Unit();
    }
    Unit ScriptEngine::ITerminate() { return Unit(); }

    S_METHOD_IMPL(ScriptEngine, Unit, DeleteScript, (UUID id), (id))
    {
        m_ScriptStates.erase(id);
        m_CreateFuncs.erase(id);
        return Unit();
    }
    S_METHOD_IMPL(ScriptEngine, Unit, Modify, (UUID id), (id))
    {
        if (m_ScriptStates.contains(id))
        {
            m_ScriptStates[id].modified = true;
        }
        return Unit();
    }
    S_METHOD_IMPL(ScriptEngine, Unit, Clear, (), ())
    {
        m_CreateFuncs.clear();
        m_ScriptStates.clear();
        m_ScriptLibrary = nullptr;
        return Unit();
    }

    S_METHOD_IMPL(ScriptEngine, bool, ReloadSripts, (), ())
    {
        if (m_ScriptStates.empty())
        {
            return false;
        }
        std::vector<Path> objects_list;
        if (!SourcesExist() || !UnModifiedObjectsExist())
        {
            return false;
        }

        for (const auto& [id, state] : m_ScriptStates)
        {
            objects_list.push_back(PathToCompiledScript(state.path));
            if (state.modified)
            {
                if (!Compiler::Compile(state.path, objects_list.back()))
                {
                    return false;
                }
            }
        }
        std::string library_name = "ScriptsLibrary";
        m_ScriptLibrary          = CreateScope<SharedObject>();
        if (!Compiler::Link(objects_list, Config::GetPath(DE_CFG_SCRIPT_CACHE_PATH), library_name))
        {
            return false;
        }
        if (!m_ScriptLibrary->Load(Config::GetPath(DE_CFG_SCRIPT_CACHE_PATH), library_name))
        {
            return false;
        }
        bool res = UpdateFuncTable();
        return res;
    }
    S_METHOD_IMPL(ScriptEngine, bool, Valid, (UUID id), (id)) { return m_CreateFuncs.contains(id); }
    S_METHOD_IMPL(ScriptEngine, bool, AddScript, (const ScriptAsset& asset), (asset))
    {
        if (!FileSystem::IsSubPath(asset.path, Config::GetPath(DE_CFG_SCRIPT_PATH)))
        {
            return false;
        }
        if (m_ScriptStates.contains(asset.id))
        {
            return false;
        }
        m_ScriptStates[asset.id].modified = true;
        m_ScriptStates[asset.id].path     = asset.path;
        m_CreateFuncs[asset.id]           = nullptr;

        return true;
    }

    S_METHOD_IMPL(ScriptEngine, Ref<Script>, CreateScript, (UUID id), (id))
    {
        if (!m_CreateFuncs.contains(id))
        {
            return nullptr;
        }
        return m_CreateFuncs[id]();
    }

    bool ScriptEngine::SourcesExist() const
    {
        for (const auto& [id, state] : m_ScriptStates)
        {
            if (!fs::exists(state.path))
            {
                return false;
            }
        }
        return true;
    }
    bool ScriptEngine::UnModifiedObjectsExist() const
    {
        for (const auto& [id, state] : m_ScriptStates)
        {
            if (state.modified)
            {
                continue;
            }
            if (!fs::exists(PathToCompiledScript(state.path)))
            {
                return false;
            }
        }
        return true;
    }
    bool ScriptEngine::UpdateFuncTable()
    {
        std::unordered_map<UUID, CreateScriptFunc> res;
        for (const auto& [id, state] : m_ScriptStates)
        {
            CreateScriptFunc create_func =
                (CreateScriptFunc)m_ScriptLibrary->GetFunction("CreateInstance" + state.path.stem().string());
            if (!create_func)
            {
                return false;
            }
            res[id] = create_func;
            ScriptClass s_class(state.path.stem().string());
            s_class.Load(m_ScriptLibrary);
            LOG_INFO(StrCat("Class ", state.path.stem().string(), " ", (s_class.Valid() ? "Valid" : "Invalid")), "ScriptEngine");
        }
        m_CreateFuncs = std::move(res);
        return true;
    }

}  // namespace DE