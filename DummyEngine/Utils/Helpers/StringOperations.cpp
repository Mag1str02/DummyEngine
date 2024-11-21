#include "StringOperations.h"

#include <sstream>

namespace DummyEngine {

    U32 HexCharToInt(char a) {
        if (a >= 'a') {
            return a - 'a' + 10;
        }
        if (a >= 'A') {
            return a - 'A' + 10;
        }
        return a - '0';
    }
    U64 StrToHex(std::string_view string) {
        U64 res = 0;
        for (size_t i = 0; i < string.size(); ++i) {
            res <<= 4;
            res += HexCharToInt(string[i]);
        }
        return res;
    }
    std::string HexToStr(U64 a) {
        std::stringstream res;
        res << std::setfill('0') << std::setw(16) << std::hex << a;
        return res.str();
    }
}  // namespace DummyEngine
