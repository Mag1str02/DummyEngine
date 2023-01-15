#include "DummyEngine/Core/Scripting/ScriptManager.h"
#include "DummyEngine/Core/Scripting/Compiler.h"
#include <thread>

namespace DE
{
    ScriptManager* ScriptManager::manager = nullptr;
    ScriptManager& ScriptManager::Get()
    {
        if (!manager)
        {
            manager = new ScriptManager();
        }
        return *manager;
        // TODO: Move instance to Application or init. Currently memory leak.
    }

    Path PathToCompiledScript(Path path)
    {
        return Config::GetPath(DE_CFG_SCRIPT_CACHE_PATH) / (fs::relative(path, Config::GetPath(DE_CFG_SCRIPT_PATH)).string() + ".o");
    }

    bool ScriptManager::SourcesExist() const
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
    bool ScriptManager::UnModifiedObjectsExist() const
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

    bool ScriptManager::UpdateFuncTable()
    {
        std::unordered_map<UUID, CreateScriptInstanceFunc> res;
        for (const auto& [id, state] : m_ScriptStates)
        {
            CreateScriptInstanceFunc create_func =
                (CreateScriptInstanceFunc)m_ScriptLibrary->GetFunction("CreateInstance" + state.path.stem().string());
            if (!create_func)
            {
                return false;
            }
            res[id] = create_func;
        }
        m_CreateFuncs = std::move(res);
        return true;
    }

    bool ScriptManager::AddScript(const ScriptAsset& asset)
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
    void ScriptManager::DeleteScript(UUID id)
    {
        m_ScriptStates.erase(id);
        m_CreateFuncs.erase(id);
    }
    void ScriptManager::Modify(UUID id)
    {
        if (m_ScriptStates.contains(id))
        {
            m_ScriptStates[id].modified = true;
        }
    }
    bool ScriptManager::ReloadSripts()
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
    bool                ScriptManager::Valid(UUID id) { return m_CreateFuncs.contains(id); }
    Ref<ScriptInstance> ScriptManager::CreateScriptInstance(UUID id)
    {
        if (!m_CreateFuncs.contains(id))
        {
            return nullptr;
        }
        return m_CreateFuncs[id]();
    }
    void ScriptManager::Clear()
    {
        m_CreateFuncs.clear();
        m_ScriptStates.clear();
        m_ScriptLibrary = nullptr;
    }

    ScriptManager::ScriptManager() { m_ScriptLibrary = CreateScope<SharedObject>(); }
    ScriptManager::~ScriptManager() {}
}  // namespace DE