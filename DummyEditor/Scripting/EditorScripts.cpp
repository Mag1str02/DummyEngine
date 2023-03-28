#include "DummyEditor/Scripting/EditorScripts.h"

namespace DE {
    std::vector<ScriptAsset> g_EditorScriptAssets = {
        {UUID(0xf001), "EditorCameraController", Path()}
    };
    std::unordered_map<std::string, UUID> g_EditorScriptNameToId = {
        {"EditorCameraController", UUID(0xf001)}
    };
}  // namespace DE