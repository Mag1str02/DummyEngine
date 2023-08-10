#include "DummyEngine/Core/Application/FileSystem.h"

#include <nfd.h>

namespace DE {

    using namespace std::string_literals;

    Path FileSystem::OpenFileDialog(const std::string& description, const std::string& filter, const Path& default_path) {
        Path path = Path();
        NFD_Init();
        nfdchar_t*      outPath;
        nfdfilteritem_t filterItem[1] = {
            {description.c_str(), filter.c_str()}
        };
        nfdresult_t result = NFD_OpenDialog(&outPath, filterItem, 1, default_path.empty() ? nullptr : default_path.string().c_str());
        if (result == NFD_OKAY) {
            path = Path(outPath);
            NFD_FreePath(outPath);
        }
        NFD_Quit();
        return path;
    }

    Path FileSystem::SaveFileDialog(const std::string& description,
                                    const std::string& filter,
                                    const std::string& default_name,
                                    const Path&        default_path) {
        Path path = Path();
        NFD_Init();
        nfdchar_t*      outPath;
        nfdfilteritem_t filterItem[1] = {
            {description.c_str(), filter.c_str()}
        };
        nfdresult_t result = NFD_SaveDialog(&outPath,
                                            filterItem,
                                            1,
                                            (default_path.empty() ? nullptr : default_path.string().c_str()),
                                            (default_name.empty() ? nullptr : default_name.c_str()));
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