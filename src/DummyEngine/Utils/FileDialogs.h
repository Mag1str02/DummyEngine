#pragma once

#include "DummyEngine/Utils/Types.h"

namespace DE
{
    class FileDialogs
    {
    public:
        static Path OpenFile(const std::string& description, const std::string& filter);
        static Path SaveFile(const char* filter);
    };

}  // namespace DE