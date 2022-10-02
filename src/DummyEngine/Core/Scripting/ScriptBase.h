#include "Addition/Base.h"

namespace DE
{
    class ScriptBase
    {
    public:
        ScriptBase(const fs::path& path_to_dll);
        ScriptBase(ScriptBase&&);
        ScriptBase& operator=(ScriptBase&&);
        ~ScriptBase();

        ScriptBase(const ScriptBase&) = delete;
        ScriptBase& operator=(const ScriptBase&) = delete;

    private:
        HMODULE m_Handle;
    };
}  // namespace DE