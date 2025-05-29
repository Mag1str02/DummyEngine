#include "Compiler.h"

namespace DummyEngine {

    SINGLETON_BASE(Compiler);

    S_INITIALIZE() {
        impl_ = CreateCompilerImpl();
        AddIncludeDir("..");
        AddIncludeDir("../Dependencies/GLM");
        AddIncludeDir("../Dependencies/TRACY/public");
        AddIncludeDir("../Dependencies/DUMMY_CONCURRENCY");
        AddLinkLibrary("DummyEngineLib");
        AddLinkLibrary("TracyClient");
        AddLinkLibrary("DummyConcurrency");
        AddDefine("DE_ENABLE_LOGGING=" + std::to_string(DE_ENABLE_LOGGING));
        AddDefine("DE_ENABLE_ASSERTS=" + std::to_string(DE_ENABLE_ASSERTS));
        AddDefine("DE_LIBRARY_CONSUMER");
        AddDefine("GLM_ENABLE_EXPERIMENTAL");

#if DE_ENABLE_PROFILER
        AddDefine("DE_ENABLE_PROFILER");
#endif
#if TRACY_ENABLE
        AddDefine("TRACY_ENABLE");
#endif

        AddDefine("TRACY_DELAYED_INIT");
        AddDefine("TRACY_FIBERS");
        AddDefine("TRACY_IMPORTS");
        AddDefine("TRACY_MANUAL_LIFETIME");
        AddDefine("TRACY_TIMER_FALLBACK");
        AddDefine("TRACY_NO_CALLSTACK");
        AddDefine("TRACY_NO_CALLSTACK_INLINES");
        AddDefine("TRACY_NO_VSYNC_CAPTURE");
        AddDefine("TRACY_NO_FRAME_IMAGE");

        return Unit();
    }
    S_TERMINATE() {
        return Unit();
    }
    S_METHOD_IMPL(bool, Compile, (const Path& source, const Path& destination), (source, destination)) {
        return impl_->Compile(source, destination);
    }
    S_METHOD_IMPL(bool,
                  Link,
                  (const std::vector<Path>& sources, const Path& destination, const std::string& library_name),
                  (sources, destination, library_name)) {
        return impl_->Link(sources, destination, library_name);
    }

    S_METHOD_IMPL(Unit, AddIncludeDir, (const Path& dir), (dir)) {
        impl_->AddIncludeDir(dir);
        return Unit();
    }
    S_METHOD_IMPL(Unit, DeleteIncludeDir, (const Path& dir), (dir)) {
        impl_->DeleteIncludeDir(dir);
        return Unit();
    }
    S_METHOD_IMPL(Unit, AddLinkLibrary, (const Path& dir), (dir)) {
        impl_->AddLinkLibrary(dir);
        return Unit();
    }
    S_METHOD_IMPL(Unit, DeleteLinkLibrary, (const Path& dir), (dir)) {
        impl_->DeleteLinkLibrary(dir);
        return Unit();
    }
    S_METHOD_IMPL(Unit, AddDefine, (const std::string& dir), (dir)) {
        impl_->AddDefine(dir);
        return Unit();
    }
    S_METHOD_IMPL(Unit, DeleteDefine, (const std::string& dir), (dir)) {
        impl_->DeleteDefine(dir);
        return Unit();
    }

}  // namespace DummyEngine