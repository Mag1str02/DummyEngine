#include "DummyEngine/Utils/Helpers/StringOperations.h"

#include "DummyEngine/Utils/Base/STDIncludes.h"

bool StartsWith(std::string_view input, std::string_view text) {
    if (input.size() < text.size()) {
        return false;
    }
    return input.substr(0, text.size()) == text;
}

bool EndsWith(std::string_view input, std::string_view text) {
    if (input.size() < text.size()) {
        return false;
    }
    input.remove_prefix(input.size() - text.size());
    return input == text;
}

std::string_view StripPrefix(std::string_view input, std::string_view prefix) {
    if (StartsWith(input, prefix)) {
        input.remove_prefix(prefix.size());
    }
    return input;
}
std::string_view StripSuffix(std::string_view input, std::string_view suffix) {
    if (EndsWith(input, suffix)) {
        input.remove_suffix(suffix.size());
    }
    return input;
}

std::string_view ClippedSubstr(std::string_view s, size_t pos, size_t n) {
    if (pos + n > s.size()) {
        return s;
    }
    s.remove_prefix(pos);
    s.remove_suffix(s.size() - n);
    return s;
}
std::string_view StripAsciiWhitespace(std::string_view s) {
    while (!s.empty() && std::isspace(s[0])) {
        s.remove_prefix(1);
    }
    while (!s.empty() && std::isspace(s.back())) {
        s.remove_suffix(1);
    }
    return s;
}

std::string_view RemoveSlash(std::string_view path) {
    if (path.size() == 1) {
        return path;
    }
    if (path.back() == '/') {
        path.remove_suffix(1);
    }
    return path;
}
std::string AddSlash(std::string_view path) {
    std::string answer;
    if (path.empty() || path.back() != '/') {
        answer.reserve(path.size() + 1);
        answer.assign(path.begin(), path.end());
        answer.push_back('/');
    } else {
        answer = {path.data(), path.size()};
    }
    return answer;
}

std::string_view Basename(std::string_view path) {
    path.remove_prefix(path.find_last_of('/') + 1);
    return path;
}
std::string_view Dirname(std::string_view path) {
    path.remove_suffix(Basename(path).size());
    if (path.size() != 1) {
        path.remove_suffix(1);
    }
    return path;
}

std::string CollapseSlashes(std::string_view path) {
    size_t      new_size = 1;
    size_t      new_pos  = 1;
    std::string answer;
    if (path.empty()) {
        return answer;
    }

    for (size_t i = 1; i < path.size(); ++i) {
        if (path[i] != '/' || path[i - 1] != '/') {
            ++new_size;
        }
    }

    answer.resize(new_size);
    answer[0] = path[0];
    for (size_t i = 1; i < path.size(); ++i) {
        if (path[i] != '/' || path[i - 1] != '/') {
            answer[new_pos++] = path[i];
        }
    }
    return answer;
}

std::string StrJoin(const std::vector<std::string_view>& strings, std::string_view delimiter) {
    std::string answer;
    if (strings.empty()) {
        return answer;
    }

    size_t cur_pos  = 0;
    size_t new_size = delimiter.size() * (strings.size() - 1);

    for (const auto& string : strings) {
        new_size += string.size();
    }
    answer.reserve(new_size);
    for (const auto& string : strings) {
        answer.replace(answer.begin() + cur_pos, answer.begin() + cur_pos + string.size(), string.begin(), string.end());
        cur_pos += string.size();
        if (cur_pos != new_size) {
            answer.replace(answer.begin() + cur_pos, answer.begin() + cur_pos + delimiter.size(), delimiter.begin(), delimiter.end());
            cur_pos += delimiter.size();
        }
    }
    return answer;
}

std::vector<std::string_view> StrSplit(std::string_view text, std::string_view delimiter) {
    size_t           delimiter_amount = 0;
    std::string_view copy             = text;
    size_t           pos;
    while ((pos = copy.find(delimiter)) != std::string::npos) {
        ++delimiter_amount;
        copy.remove_prefix(pos + delimiter.size());
    }
    std::vector<std::string_view> answer(delimiter_amount + 1);
    size_t                        i = 0;
    while ((pos = text.find(delimiter)) != std::string::npos) {
        answer[i++] = text.substr(0, pos);
        text.remove_prefix(pos + delimiter.size());
    }
    answer[i++] = text;

    return answer;
}

std::string StrCat() {
    return std::string();
}

template <> size_t StrLen<int>(const int& value) {
    size_t    len  = (value < 0) ? 1 : 0;
    long long buff = value;
    do {
        ++len;
        buff /= 10;
    } while (buff != 0);
    // std::cout << "I len" << len << std::endl;
    return len;
}
template <> size_t StrLen<long>(const long& value) {
    size_t    len  = (value < 0) ? 1 : 0;
    long long buff = value;
    do {
        ++len;
        buff /= 10;
    } while (buff != 0);
    // std::cout << "L len" << len << std::endl;
    return len;
}
template <> size_t StrLen<long long>(const long long& value) {
    size_t    len  = (value < 0) ? 1 : 0;
    long long buff = value;
    do {
        ++len;
        buff /= 10;
    } while (buff != 0);
    // std::cout << "LL len" << len << std::endl;
    return len;
}

template <> size_t StrLen<unsigned int>(const unsigned int& value) {
    size_t len  = 0;
    size_t buff = value;
    do {
        ++len;
        buff /= 10;
    } while (buff != 0);
    // std::cout << "UI len" << len << std::endl;
    return len;
}
template <> size_t StrLen<unsigned long>(const unsigned long& value) {
    size_t len  = 0;
    size_t buff = value;
    do {
        ++len;
        buff /= 10;
    } while (buff != 0);
    // std::cout << "UL len" << len << std::endl;
    return len;
}
template <> size_t StrLen<unsigned long long>(const unsigned long long& value) {
    size_t len  = 0;
    size_t buff = value;
    do {
        ++len;
        buff /= 10;
    } while (buff != 0);
    // std::cout << "ULL len" << len << std::endl;
    return len;
}
template <> size_t StrLen<std::string>(const std::string& value) {
    return value.size();
}
template <> size_t StrLen<std::string_view>(const std::string_view& value) {
    return value.size();
}
template <> size_t StrLen<std::filesystem::path>(const std::filesystem::path& value) {
    return value.native().size();
}

size_t StrLen(const char* str) {
    return std::strlen(str);
}

template <> void Append<int>(std::string& input, const int& value, size_t start) {
    if (value < 0) {
        input[start++] = '-';
    }
    long long buff = value;
    // std::cout << "Appending" << std::to_string(value) << "|" << buff << std::endl;
    size_t digits[64];
    size_t digit_pos = 0;
    do {
        digits[digit_pos++] = (buff % 10) * (buff < 0 ? -1 : 1);
        buff /= 10;
    } while (buff != 0);
    for (size_t i = 0; i < digit_pos; ++i) {
        input[start + digit_pos - i - 1] = '0' + digits[i];
    }
}
template <> void Append<long>(std::string& input, const long& value, size_t start) {
    if (value < 0) {
        input[start++] = '-';
    }
    long long buff = value;
    size_t    digits[64];
    size_t    digit_pos = 0;
    do {
        digits[digit_pos++] = (buff % 10) * (buff < 0 ? -1 : 1);
        buff /= 10;
    } while (buff != 0);
    for (size_t i = 0; i < digit_pos; ++i) {
        input[start + digit_pos - i - 1] = '0' + digits[i];
    }
}
template <> void Append<long long>(std::string& input, const long long& value, size_t start) {
    if (value < 0) {
        input[start++] = '-';
    }
    long long buff = value;
    // std::cout << value << "|" << buff << std::endl;
    size_t digits[64];
    size_t digit_pos = 0;
    do {
        digits[digit_pos++] = (buff % 10) * (buff < 0 ? -1 : 1);
        buff /= 10;
    } while (buff != 0);
    for (size_t i = 0; i < digit_pos; ++i) {
        input[start + digit_pos - i - 1] = '0' + digits[i];
    }
}
template <> void Append<unsigned int>(std::string& input, const unsigned int& value, size_t start) {
    unsigned long long buff = value;
    size_t             digits[64];
    size_t             digit_pos = 0;
    do {
        digits[digit_pos++] = buff % 10;
        buff /= 10;
    } while (buff != 0);
    for (size_t i = 0; i < digit_pos; ++i) {
        input[start + digit_pos - i - 1] = '0' + digits[i];
    }
}
template <> void Append<unsigned long>(std::string& input, const unsigned long& value, size_t start) {
    unsigned long long buff = value;
    size_t             digits[64];
    size_t             digit_pos = 0;
    do {
        digits[digit_pos++] = buff % 10;
        buff /= 10;
    } while (buff != 0);
    for (size_t i = 0; i < digit_pos; ++i) {
        input[start + digit_pos - i - 1] = '0' + digits[i];
    }
}
template <> void Append<unsigned long long>(std::string& input, const unsigned long long& value, size_t start) {
    unsigned long long buff = value;
    // std::cout << value << "|" << buff << std::endl;
    size_t digits[64];
    size_t digit_pos = 0;
    do {
        digits[digit_pos++] = buff % 10;
        buff /= 10;
    } while (buff != 0);
    for (size_t i = 0; i < digit_pos; ++i) {
        input[start + digit_pos - i - 1] = '0' + digits[i];
    }
}
template <> void Append<std::string>(std::string& input, const std::string& value, size_t start) {
    for (size_t i = 0; i < value.size(); ++i) {
        input[start + i] = value[i];
    }
}
template <> void Append<std::string_view>(std::string& input, const std::string_view& value, size_t start) {
    for (size_t i = 0; i < value.size(); ++i) {
        input[start + i] = value[i];
    }
}
template <> void Append<std::filesystem::path>(std::string& input, const std::filesystem::path& value, size_t start) {
    for (size_t i = 0; i < value.native().size(); ++i) {
        input[start + i] = value.native()[i];
    }
}

void Append(std::string& input, const char* value, size_t start) {
    size_t len = strlen(value);
    for (size_t i = 0; i < len; ++i) {
        input[start + i] = value[i];
    }
}
