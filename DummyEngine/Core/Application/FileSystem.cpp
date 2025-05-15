#include "FileSystem.h"

#include <nfd.h>

namespace DummyEngine {

    using namespace std::string_literals;

    Path FileSystem::OpenFileDialog(const std::string& description, const std::string& filter, const Path& default_path) {
        DE_PROFILE_SCOPE("FileSystem::OpenFileDialog");
        Path path = Path();
        NFD_Init();
        nfdchar_t*      out_path;
        nfdfilteritem_t filter_item[1] = {
            {description.c_str(), filter.c_str()}
        };
        nfdresult_t result = NFD_OpenDialog(&out_path, filter_item, 1, default_path.empty() ? nullptr : default_path.string().c_str());
        if (result == NFD_OKAY) {
            path = Path(out_path);
            NFD_FreePath(out_path);
        }
        NFD_Quit();
        return path;
    }

    Path FileSystem::SaveFileDialog(const std::string& description,
                                    const std::string& filter,
                                    const std::string& default_name,
                                    const Path&        default_path) {
        DE_PROFILE_SCOPE("FileSystem::SaveFileDialog");
        Path path = Path();
        NFD_Init();
        nfdchar_t*      out_path;
        nfdfilteritem_t filter_item = {description.c_str(), filter.c_str()};

        nfdresult_t result = NFD_SaveDialog(&out_path,
                                            &filter_item,
                                            1,
                                            (default_path.empty() ? nullptr : default_path.string().c_str()),
                                            (default_name.empty() ? nullptr : default_name.c_str()));
        if (result == NFD_OKAY) {
            std::string s(out_path);
            path = Path(s);
            NFD_FreePath(out_path);
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

}  // namespace DummyEngine