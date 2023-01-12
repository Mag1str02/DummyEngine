#include "DummyEngine/Utils/Base.h"

namespace DE
{
    class CompilerImpl;

    class Compiler
    {
    public:
        static int Compile(const Path& source, const Path& destination, const std::string filename_without_extension);

    private:
        static Scope<CompilerImpl> m_Impl;
    };
}  // namespace DE