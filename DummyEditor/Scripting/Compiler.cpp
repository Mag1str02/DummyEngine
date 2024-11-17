#include "DummyEditor/Scripting/Compiler.h"

namespace DE {
    SINGLETON_BASE(Compiler);

    S_INITIALIZE() {
        m_Impl = CreateCompilerImpl();
        AddIncludeDir("..");
        AddIncludeDir("../Dependencies/GLM");
        AddLinkLibrary("DummyEngineLib");
        AddDefine("DE_ENABLE_LOGGING=" + std::to_string(DE_ENABLE_LOGGING));
        AddDefine("DE_ENABLE_ASSERTS=" + std::to_string(DE_ENABLE_ASSERTS));
        AddDefine("DE_ENABLE_PROFILER=" + std::to_string(DE_ENABLE_PROFILER));

        return Unit();
    }
    S_TERMINATE() {
        return Unit();
    }
    S_METHOD_IMPL(bool, Compile, (const Path& source, const Path& destination), (source, destination)) {
        return m_Impl->Compile(source, destination);
    }
    S_METHOD_IMPL(bool,
                  Link,
                  (const std::vector<Path>& sources, const Path& destination, const std::string& library_name),
                  (sources, destination, library_name)) {
        return m_Impl->Link(sources, destination, library_name);
    }

    S_METHOD_IMPL(Unit, AddIncludeDir, (const Path& dir), (dir)) {
        m_Impl->AddIncludeDir(dir);
        return Unit();
    }
    S_METHOD_IMPL(Unit, DeleteIncludeDir, (const Path& dir), (dir)) {
        m_Impl->DeleteIncludeDir(dir);
        return Unit();
    }
    S_METHOD_IMPL(Unit, AddLinkLibrary, (const Path& dir), (dir)) {
        m_Impl->AddLinkLibrary(dir);
        return Unit();
    }
    S_METHOD_IMPL(Unit, DeleteLinkLibrary, (const Path& dir), (dir)) {
        m_Impl->DeleteLinkLibrary(dir);
        return Unit();
    }
    S_METHOD_IMPL(Unit, AddDefine, (const std::string& dir), (dir)) {
        m_Impl->AddDefine(dir);
        return Unit();
    }
    S_METHOD_IMPL(Unit, DeleteDefine, (const std::string& dir), (dir)) {
        m_Impl->DeleteDefine(dir);
        return Unit();
    }

}  // namespace DE