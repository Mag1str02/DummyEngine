#pragma once

#include "DummyEngine/Utils/Types.h"

namespace DE
{
    class FileSystem
    {
    public:
        static Path OpenFileDialog(const std::string& description, const std::string& filter);
        static Path SaveFileDialog(const std::string& description, const std::string& filter);
    };

}  // namespace DE