#include "Addition/Base.h"
#include "Core/Scripting/ScriptBase.h"

namespace DE
{
    class ScriptManager
    {
    public:
        static bool UploadScript(UUID id, const fs::path& path);

    private:
        static bool LoadScript(UUID id, const fs::path& path);
        static void UnloadScript(UUID id);

        struct ScriptManagerState
        {
            std::unordered_map<UUID, ScriptBase> m_Scripts;
        };
        static ScriptManagerState m_State;
    };
}  // namespace DE