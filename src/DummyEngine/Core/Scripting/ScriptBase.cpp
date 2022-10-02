#include "Core/Scripting/ScriptBase.h"

namespace DE
{
    ScriptBase::ScriptBase(const fs::path& path_to_dll)
    {
        m_Handle = LoadLibrary(path_to_dll.string().c_str());
        if (!m_Handle)
        {
            Logger::Info("loading", "ScriptBase", "Failed to load dll: " + path_to_dll.string());
        }
        Logger::Info("loading", "ScriptBase", "Loaded dll: " + path_to_dll.string());
    }
    ScriptBase::ScriptBase(ScriptBase&& other)
    {
        m_Handle = other.m_Handle;
        other.m_Handle = HMODULE();
    }
    ScriptBase& ScriptBase::operator=(ScriptBase&& other)
    {
        m_Handle = other.m_Handle;
        other.m_Handle = HMODULE();
        return *this;
    }
    ScriptBase::~ScriptBase() { FreeLibrary(m_Handle); }
}  // namespace DE