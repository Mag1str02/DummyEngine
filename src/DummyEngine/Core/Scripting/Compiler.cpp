#include "Core/Scripting/Compiler.h"

namespace DE
{
    int Compiler::Compile(const Path& path_to_source)
    {
        std::string compile_command = GenCompileCommand(path_to_source);
        std::cout << compile_command << std::endl;
        return system(compile_command.c_str());
    }
    std::string Compiler::GenCompileCommand(const Path& path_to_source)
    {
        std::string source_file = fs::relative(path_to_source).string();
        std::string dll_file = (Config::GetPath(DE_CFG_SCRIPT_CACHE_PATH) / fs::relative(path_to_source, Config::GetPath(DE_CFG_SCRIPT_PATH)) / ".." /
                                path_to_source.stem())
                                   .string();

        std::replace(source_file.begin(), source_file.end(), '\\', '/');
        std::replace(dll_file.begin(), dll_file.end(), '\\', '/');
        return "g++ --shared " + source_file + " -I ../src/DummyEngine -I ../src/DummyEngine/Libs/GLM -L . -l DummyEngineLib -o " + dll_file + ".dll";
    }
}  // namespace DE