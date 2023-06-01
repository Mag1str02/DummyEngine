#include "DummyEngine/Core/Application/FileSystem.h"

#include <nfd.h>

namespace DE {

    using namespace std::string_literals;

    Path FileSystem::OpenFileDialog(const std::string& description, const std::string& filter) {
        Path path = Path();
        NFD_Init();
        nfdchar_t*      outPath;
        nfdfilteritem_t filterItem[1] = {
            {description.c_str(), filter.c_str()}
        };
        nfdresult_t result = NFD_OpenDialog(&outPath, filterItem, 1, nullptr);
        if (result == NFD_OKAY) {
            path = Path(outPath);
            NFD_FreePath(outPath);
        }
        NFD_Quit();
        return path;
    }

    Path FileSystem::SaveFileDialog(const std::string& description, const std::string& filter) {
        Path path = Path();
        NFD_Init();
        nfdchar_t*      outPath;
        nfdfilteritem_t filterItem[1] = {
            {description.c_str(), filter.c_str()}
        };
        nfdresult_t result = NFD_SaveDialog(&outPath, filterItem, 1, nullptr, nullptr);
        if (result == NFD_OKAY) {
            std::string s(outPath);
            path = Path(s);
            NFD_FreePath(outPath);
        }
        NFD_Quit();
        return path;
    }
    bool FileSystem::IsSubPath(const Path& path, const Path& base) {
        std::string relative = fs::relative(path, base).string();
        return !(relative.size() > 1 && relative[0] == '.' && relative[1] == '.');
    }
    bool FileSystem::CreateDirectory(const Path& path) {
        return fs::create_directories(path);
    }
}  // namespace DE