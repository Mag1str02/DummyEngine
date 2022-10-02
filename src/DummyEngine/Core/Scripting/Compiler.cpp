#include "Core/Scripting/Compiler.h"

namespace DE
{
    int Compiler::Compile(const Path& file_path, const Path& output_file_name)
    {
        int res = system(GenCompileCommand(file_path, output_file_name).c_str());
        return res;
    }

    std::string Compiler::GenCompileCommand(const Path& file_path, const Path& output_file_name)
    {
        std::string compiler_command = "g++ --shared " + file_path.string() + " -L . -l DummyEngineLib -o " + output_file_name.string();
        return compiler_command;
    }
}  // namespace DE
