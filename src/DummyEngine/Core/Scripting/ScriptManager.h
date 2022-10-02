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

        static int         Compile(const Path& path_to_source);
        static std::string GenCompileCommand(const Path& path_to_source);

        struct ScriptManagerState
        {
            std::unordered_map<UUID, ScriptBase> m_Scripts;
        };
        static ScriptManagerState m_State;
    };
}  // namespace DE