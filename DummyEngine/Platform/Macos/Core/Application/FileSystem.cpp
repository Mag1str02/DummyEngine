#include "DummyEngine/Core/Application/FileSystem.h"
#include <nfd.h>

namespace DE
{

    Path FileSystem::OpenFileDialog(const std::string& description, const std::string& filter)
    {
        Path path = Path();
        NFD_Init();
        nfdchar_t *outPath;
        nfdfilteritem_t filterItem[1] = { { description.c_str(), filter.c_str() } };
        nfdresult_t result = NFD_OpenDialog(&outPath, filterItem, 1, nullptr);
        if (result == NFD_OKAY)
        {
            path = Path(outPath);
            NFD_FreePath(outPath);
        }
        NFD_Quit();
        return path;
    }

    Path FileSystem::SaveFileDialog(const std::string& description, const std::string& filter) {
        Path path = Path();
        NFD_Init();
        nfdchar_t *outPath;
        nfdfilteritem_t filterItem[1] = { { description.c_str(), filter.c_str() } };
        nfdresult_t result = NFD_SaveDialog(&outPath, filterItem, 1, nullptr, nullptr);
        if (result == NFD_OKAY)
        {
            std::string s(outPath);
            path = Path(s);
            NFD_FreePath(outPath);
        }
        NFD_Quit();
        return path;
    }

}  // namespace DE