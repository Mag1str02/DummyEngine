#include "DummyEngine/Core/Application/FileSystem.h"

namespace DE {

    using namespace std::string_literals;

    Path FileSystem::OpenFileDialog(const std::string& description, const std::string& filter) {
        return Path();
    }

    Path FileSystem::SaveFileDialog(const std::string& description, const std::string& filter) {
        return Path();
    }
}  // namespace DE