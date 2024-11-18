#pragma once
#include "DummyEngine/Utils/Base/STDIncludes.h"
#include "DummyEngine/Utils/Types/Types.h"

namespace DE {

    U32         HexCharToInt(char a);
    U64         StrToHex(std::string_view string);
    std::string HexToStr(U64 a);

}  // namespace DE
