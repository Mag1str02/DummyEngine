#pragma once

#include "DummyEngine/Addition/Types.h"

namespace DE
{
    class FileDialogs
    {
    public:
        static Path OpenFile(const char* filter);
        static Path SaveFile(const char* filter);
    };

}  // namespace DE