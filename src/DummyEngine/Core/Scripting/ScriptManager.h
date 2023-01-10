#include "DummyEngine/Addition/Base.h"
#include "DummyEngine/Core/ResourceManaging/Assets.h"
#include "DummyEngine/Core/Scripting/ScriptInstance.h"

namespace DE
{
    using CreateScriptInstanceFunc = Ref<ScriptInstance> (*)();
    class ScriptFile
    {
    public:
        ScriptFile();
        ScriptFile(const ScriptAsset& asset);
        ScriptFile(ScriptFile&& other);
        ScriptFile& operator=(ScriptFile&& other);
        ~ScriptFile();

        bool Valid() const;
        Ref<ScriptInstance> CreateScriptInstance() const;

    private:
        bool DLLAlreadyExists(const std::string& path_to_dll) const;

        HMODULE                  m_DLLHandle;
        CreateScriptInstanceFunc m_CreateScriptInstance;
    };

    class ScriptManager
    {
    public:
        static void                UploadScript(const ScriptAsset& asset);
        static bool                HasScript(UUID id);
        static Ref<ScriptInstance> CreateScriptInstance(UUID id);
        static void Clear();

    private:
        struct ScriptManagerState
        {
            std::unordered_map<UUID, ScriptFile> m_Scripts;
        };
        static ScriptManagerState m_State;
    };
}  // namespace DE