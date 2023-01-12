#include "DummyEngine/Utils/FileDialogs.h"

namespace DE
{

    Path FileDialogs::OpenFile(const std::string& description, const std::string& filter) { return Path(); }

    Path FileDialogs::SaveFile(const char* filter) { return Path(); }
}  // namespace DE