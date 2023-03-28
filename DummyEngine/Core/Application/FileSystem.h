#pragma once

#include "DummyEngine/Utils/Base.h"

namespace DE {
    class FileSystem {
    public:
        static Path OpenFileDialog(const std::string& description, const std::string& filter);
        static Path SaveFileDialog(const std::string& description, const std::string& filter);

        static bool IsSubPath(const Path& path, const Path& base) {
            std::string relative = fs::relative(path, base).string();
            return !(relative.size() > 1 && relative[0] == '.' && relative[1] == '.');
        }
        static bool CreateDirectory(const Path& path) { return fs::create_directories(path); }
    };

}  // namespace DE