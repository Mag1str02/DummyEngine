#include <iostream>
#include <filesystem>
#include "lib.h"

namespace fs = std::filesystem;

#ifdef WIN32
#include <windows.h>
#endif

typedef void (*f_funci)();

fs::path working_dir = fs::current_path();

std::string GetCompileCommand(const std::string& filename, const std::string& lib_name)
{
    std::string compiler_command = "g++ --shared ../" + filename + " -L . -l" + lib_name + " -o script.dll";
    std::cout << compiler_command << std::endl;

    return compiler_command;
}

void rt_execute()
{
    int res = system(GetCompileCommand("script.cpp", "Lib").c_str());
    std::cout << "CompileRes: " << res << std::endl;
    HMODULE hGetProcIDDLL = LoadLibrary("script.dll");
    if (!hGetProcIDDLL)
    {
        std::cout << "could not load the dynamic library" << std::endl;
        return;
    }

    f_funci funci = (f_funci)GetProcAddress(hGetProcIDDLL, "funci");
    if (!funci)
    {   
        std::cout << "could not locate the function" << std::endl;
        return;
    }
    funci();
    Print();

    FreeLibrary(hGetProcIDDLL);
    return;
}

int main()
{
    std::cout << "Main Start" << std::endl;
    std::cout << working_dir << std::endl;
    rt_execute();
    std::cout << "Main End" << std::endl;
    getchar();
    DeleteFile((working_dir / "script.dll").string().c_str());
    return 0;
}