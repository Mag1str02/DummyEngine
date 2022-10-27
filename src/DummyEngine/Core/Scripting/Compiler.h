#include "Addition/Base.h"

namespace DE
{
    class Compiler
    {
    public:
        static int Compile(const Path& path_to_source);

    private:
        static std::string GenCompileCommand(const Path& path_to_source);
        static Path        PathToDLL(const Path& path_to_source);
    };
}  // namespace DE