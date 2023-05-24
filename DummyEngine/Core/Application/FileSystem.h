#pragma once

#include "DummyEngine/Utils/Base.h"

namespace DE {
    class FileSystem {
    public:
        static Path OpenFileDialog(const std::string& description, const std::string& filter);
        static Path SaveFileDialog(const std::string& description, const std::string& filter);

        static bool IsSubPath(const Path& path, const Path& base);
        static bool CreateDirectory(const Path& path);
    };

}  // namespace DE