#include "Core/Scripting/ScriptManager.h"
#include "Core/Scripting/Compiler.h"

namespace DE
{
    ScriptManager::ScriptManagerState ScriptManager::m_State;

    ScriptFile::ScriptFile() : m_DLLHandle(HMODULE()), m_CreateScriptInstance(nullptr) {}
    ScriptFile::ScriptFile(const ScriptAsset& asset) : m_DLLHandle(HMODULE()), m_CreateScriptInstance(nullptr)
    {
        int res = Compiler::Compile(asset.path);
        if (res)
        {
            Logger::Error("loading", "ScriptFile", "Failed to compile script asset: " + std::to_string(asset.id));
            return;
        }
        std::string path_to_dll =
            (Config::GetPath(DE_CFG_SCRIPT_CACHE_PATH) / fs::relative(asset.path, Config::GetPath(DE_CFG_SCRIPT_PATH)) / ".." / asset.path.stem())
                .string() +
            ".dll";
        m_DLLHandle = LoadLibrary(path_to_dll.c_str());

        if (!m_DLLHandle)
        {
            Logger::Error("loading", "ScriptFile", "Failed to load script dll: " + path_to_dll);
            return;
        }
        m_CreateScriptInstance = (CreateScriptInstanceFunc)GetProcAddress(m_DLLHandle, "CreateInstance");
        if (!m_CreateScriptInstance)
        {
            Logger::Error("loading", "ScriptFile", "Failed to create script instance function: " + asset.path.stem().string());
            return;
        }
        Logger::Info("loading", "ScriptFile", "Script file loaded: " + RelativeToExecutable(asset.path).string());
    }
    ScriptFile::ScriptFile(ScriptFile&& other)
    {
        m_DLLHandle                  = other.m_DLLHandle;
        m_CreateScriptInstance       = other.m_CreateScriptInstance;
        other.m_DLLHandle            = HMODULE();
        other.m_CreateScriptInstance = nullptr;
    }
    ScriptFile& ScriptFile::operator=(ScriptFile&& other)
    {
        m_DLLHandle                  = other.m_DLLHandle;
        m_CreateScriptInstance       = other.m_CreateScriptInstance;
        other.m_DLLHandle            = HMODULE();
        other.m_CreateScriptInstance = nullptr;
        return *this;
    }
    ScriptFile::~ScriptFile()
    {
        if (m_DLLHandle)
        {
            FreeLibrary(m_DLLHandle);
        }
    }
    bool                ScriptFile::Valid() const { return m_CreateScriptInstance; }
    Ref<ScriptInstance> ScriptFile::CreateScriptInstance() const { return m_CreateScriptInstance(); }

    void ScriptManager::UploadScript(const ScriptAsset& asset)
    {
        if (!m_State.m_Scripts.contains(asset.id))
        {
            ScriptFile script(asset);
            if (script.Valid())
            {
                m_State.m_Scripts.emplace(asset.id, std::move(script));
            }
        }
    }
    bool                ScriptManager::HasScript(UUID id) { return m_State.m_Scripts.contains(id); }
    Ref<ScriptInstance> ScriptManager::CreateScriptInstance(UUID id)
    {
        DE_ASSERT(HasScript(id), "Creating instace of unknown script.");
        return m_State.m_Scripts[id].CreateScriptInstance();
    }
}  // namespace DE