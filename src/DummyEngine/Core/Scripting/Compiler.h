#include "Addition/Base.h"

namespace DE
{
    class Compiler
    {
    public:
        static int Compile(const Path& file_path, const Path& output_file_name);

    private:
        static std::string GenCompileCommand(const Path& file_path, const Path& output_file_name);
    };
}  // namespace DE