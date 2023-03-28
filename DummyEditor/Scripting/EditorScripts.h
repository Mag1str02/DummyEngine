#pragma once

#include "DummyEditor/DummyEngineInclude.h"

namespace DE {
    extern std::vector<ScriptAsset>              g_EditorScriptAssets;
    extern std::unordered_map<std::string, UUID> g_EditorScriptNameToId;
}  // namespace DE