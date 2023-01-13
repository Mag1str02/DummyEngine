#include "DummyEngine/Utils/Base.h"

namespace DE
{
    class CompilerImpl;

    class Compiler : public Singleton<Compiler>
    {
    public:
        static void StartUp();
        static void ShutDown();

        static bool Compile(const Path& source, const Path& destination);
        static bool Link(const std::vector<Path>& sources, const Path& destination, const std::string& library_name);
        static bool AddIncludeDir(const Path& source);
        static void DeleteIncludeDir(const Path& source);

    private:
        static Scope<CompilerImpl> m_Impl;
    };
}  // namespace DE