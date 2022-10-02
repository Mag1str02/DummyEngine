#include "Core/Scripting/ScriptManager.h"
#include "Core/Scripting/Compiler.h"

namespace DE
{
    ScriptManager::ScriptManagerState ScriptManager::m_State;

    bool ScriptManager::UploadScript(UUID id, const fs::path& path) {
        
    }

    bool ScriptManager::LoadScript(UUID id, const fs::path& path)
    {
        Path output_file_path = Config::GetPath(DE_CFG_SCRIPT_CACHE_PATH) / ("s_" + path.stem().string() + ".dll");
        if (!Compiler::Compile(path, output_file_path))
        {
            return false;
        }
        m_State.m_Scripts[id] = ScriptBase(output_file_path);
        return true;
    }
    void ScriptManager::UnloadScript(UUID id) { m_State.m_Scripts.erase(id); }
}  // namespace DE