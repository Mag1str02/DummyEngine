#include "DummyEditor/DummyEngineInclude.h"

namespace DE {
    class CompilerImpl;

    // TODO: Proper singleton

    class Compiler {
    public:
        static void Initialize();
        static void Terminate();

        static bool Compile(const Path& source, const Path& destination);
        static bool Link(const std::vector<Path>& sources, const Path& destination, const std::string& library_name);
        static void AddIncludeDir(const Path& source);
        static void AddDefine(const std::string& source);
        static void AddLinkLibrary(const Path& source);
        static void DeleteIncludeDir(const Path& source);
        static void DeleteDefine(const std::string& source);
        static void DeleteLinkLibrary(const Path& source);

    private:
        static Scope<CompilerImpl> m_Impl;
    };
}  // namespace DE