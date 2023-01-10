#include "DummyEngine/Core/Scripting/Compiler.h"

namespace DE
{

    int Compiler::Compile(const Path& path_to_source)
    {
        std::string compile_command = GenCompileCommand(path_to_source);
        fs::create_directories(PathToDLL(path_to_source));

        std::cout << compile_command << std::endl;

        return system(compile_command.c_str());
    }
    std::string Compiler::GenCompileCommand(const Path& path_to_source)
    {
        std::string source_file = fs::relative(path_to_source).string();
        std::string dll_file    = (PathToDLL(path_to_source) / path_to_source.stem()).string();

        std::replace(source_file.begin(), source_file.end(), '\\', '/');
        std::replace(dll_file.begin(), dll_file.end(), '\\', '/');
        return "g++ --shared " + source_file + " -I ../src/DummyEngine -I ../src/DummyEngine/Libs/GLM -L . -l DummyEngineLib -o " + dll_file + ".dll";
    }
    Path Compiler::PathToDLL(const Path& path_to_source)
    {
        return fs::weakly_canonical(Config::GetPath(DE_CFG_SCRIPT_CACHE_PATH) / fs::relative(path_to_source, Config::GetPath(DE_CFG_SCRIPT_PATH)) /
                                    "..");
    }

}  // namespace DE