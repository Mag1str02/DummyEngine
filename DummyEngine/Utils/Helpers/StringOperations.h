#pragma once

#include "DummyEngine/Utils/Types/Types.h"

#include <string>

namespace DummyEngine {

    U32         HexCharToInt(char a);
    U64         StrToHex(std::string_view string);
    std::string HexToStr(U64 a);

}  // namespace DummyEngine
