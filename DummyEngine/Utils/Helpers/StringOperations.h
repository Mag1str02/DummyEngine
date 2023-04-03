#pragma once
#include "DummyEngine/Utils/Base/STDIncludes.h"
#include "DummyEngine/Utils/Types/Types.h"

namespace DE {

    U32         HexCharToInt(char a);
    U64         StrToHex(std::string_view string);
    std::string HexToStr(U64 a);

    bool StartsWith(std::string_view string, std::string_view text);
    bool EndsWith(std::string_view string, std::string_view text);

    std::string_view StripPrefix(std::string_view string, std::string_view prefix);
    std::string_view StripSuffix(std::string_view string, std::string_view suffix);

    std::string_view ClippedSubstr(std::string_view s, size_t pos, size_t n = std::string_view::npos);
    std::string_view StripAsciiWhitespace(std::string_view s);

    std::string_view RemoveSlash(std::string_view path);
    std::string      AddSlash(std::string_view path);

    std::string_view Basename(std::string_view path);
    std::string_view Dirname(std::string_view path);

    std::string CollapseSlashes(std::string_view path);
    std::string StrJoin(const std::vector<std::string_view>& strings, std::string_view delimiter);

    std::vector<std::string_view> StrSplit(std::string_view text, std::string_view delimiter);

    std::string StrCat();

    template <typename T> void Append(std::string& input, const T& value, size_t start);
    void                       Append(std::string& input, const char* value, size_t start);

    size_t                       StrLen(const char* str);
    template <typename T> size_t StrLen(const T& value);

    template <typename T> std::string StrCatStep(size_t allocate_len, const T& t) {
        std::string answer;
        size_t      len = StrLen(t);
        answer.assign(len + allocate_len, '_');
        Append(answer, t, allocate_len);
        return answer;
    }
    template <typename T, typename... Args> std::string StrCatStep(size_t allocate_len, const T& t, const Args&... args) {
        size_t      len    = StrLen(t);
        std::string answer = StrCatStep(len + allocate_len, args...);
        Append(answer, t, allocate_len);
        return answer;
    }
    template <typename... Args> std::string StrCat(const Args&... args) {
        std::string answer = StrCatStep(0, args...);
        return answer;
    }

}  // namespace DE
