#define GLFW_EXPOSE_NATIVE_WIN32
#include <windows.h>

#include "DummyEngine/Utils/FileSystem.h"
#include "DummyEngine/Core/Application/Application.h"

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

namespace DE
{

    using namespace std::string_literals;

    Path FileSystem::OpenFileDialog(const std::string& description, const std::string& filter)
    {
        std::string filter_command;
        filter_command.reserve(description.size() + filter.size() + 3);
        filter_command.append(description);
        filter_command.append("\0"s);
        filter_command.append(filter);
        filter_command.append("\0"s);
        filter_command.append("\0"s);
        
        OPENFILENAMEA ofn;
        CHAR          szFile[260]     = {0};
        CHAR          currentDir[256] = {0};
        ZeroMemory(&ofn, sizeof(OPENFILENAME));
        ofn.lStructSize = sizeof(OPENFILENAME);
        ofn.hwndOwner   = glfwGetWin32Window(Application::Get().m_Window->m_Window);
        ofn.lpstrFile   = szFile;
        ofn.nMaxFile    = sizeof(szFile);
        if (GetCurrentDirectoryA(256, currentDir)) ofn.lpstrInitialDir = currentDir;
        ofn.lpstrFilter  = filter_command.c_str();
        ofn.nFilterIndex = 1;
        ofn.Flags        = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

        if (GetOpenFileNameA(&ofn) == TRUE) return ofn.lpstrFile;

        return Path();
    }

    Path FileSystem::SaveFileDialog(const std::string& description, const std::string& filter)
    {
        std::string filter_command;
        filter_command.reserve(description.size() + filter.size() + 3);
        filter_command.append(description);
        filter_command.append("\0"s);
        filter_command.append(filter);
        filter_command.append("\0"s);
        filter_command.append("\0"s);

        OPENFILENAMEA ofn;
        CHAR          szFile[260]     = {0};
        CHAR          currentDir[256] = {0};
        ZeroMemory(&ofn, sizeof(OPENFILENAME));
        ofn.lStructSize = sizeof(OPENFILENAME);
        ofn.hwndOwner   = glfwGetWin32Window(Application::Get().m_Window->m_Window);
        ofn.lpstrFile   = szFile;
        ofn.nMaxFile    = sizeof(szFile);
        if (GetCurrentDirectoryA(256, currentDir)) ofn.lpstrInitialDir = currentDir;
        ofn.lpstrFilter  = filter_command.c_str();
        ofn.nFilterIndex = 1;
        ofn.Flags        = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;

        // Sets the default extension by extracting it from the filter
        ofn.lpstrDefExt = strchr(filter_command.c_str(), '\0') + 1;

        if (GetSaveFileNameA(&ofn) == TRUE)
        {
            return ofn.lpstrFile;
        }

        return Path();
    }
}  // namespace DE