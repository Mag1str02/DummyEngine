#pragma once

#include "DummyEngine/Utils/Types/Types.h"

namespace DummyEngine {

    class FileSystem {
    public:
        static Path OpenFileDialog(const std::string& description, const std::string& filter, const Path& default_path = Path());
        static Path SaveFileDialog(const std::string& description,
                                   const std::string& filter,
                                   const std::string& default_name = "",
                                   const Path&        default_path = Path());

        static bool IsSubPath(const Path& path, const Path& base);
        static bool CreateDirectory(const Path& path);
    };

}  // namespace DummyEngine