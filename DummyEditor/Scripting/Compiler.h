#pragma once

#include "DummyEngine/Utils/Helpers/Singleton.h"
#include "DummyEngine/Utils/Types/Types.h"

namespace DummyEngine {
    class CompilerImpl {
    public:
        virtual ~CompilerImpl()                                                                                       = default;
        virtual bool Compile(const Path& source, const Path& destination)                                             = 0;
        virtual bool Link(const std::vector<Path>& sources, const Path& destination, const std::string& library_name) = 0;
        virtual void AddIncludeDir(const Path& source)                                                                = 0;
        virtual void AddDefine(const std::string& source)                                                             = 0;
        virtual void AddLinkLibrary(const Path& source)                                                               = 0;
        virtual void DeleteIncludeDir(const Path& source)                                                             = 0;
        virtual void DeleteDefine(const std::string& source)                                                          = 0;
        virtual void DeleteLinkLibrary(const Path& source)                                                            = 0;
    };

    class Compiler : public Singleton<Compiler> {
        SINGLETON(Compiler)
    public:
        static Scope<CompilerImpl> CreateCompilerImpl();
        S_METHOD_DEF(bool, Compile, (const Path& source, const Path& destination));
        S_METHOD_DEF(bool, Link, (const std::vector<Path>& sources, const Path& destination, const std::string& library_name));
        S_METHOD_DEF(Unit, AddIncludeDir, (const Path& source));
        S_METHOD_DEF(Unit, DeleteIncludeDir, (const Path& source));
        S_METHOD_DEF(Unit, AddLinkLibrary, (const Path& source));
        S_METHOD_DEF(Unit, DeleteLinkLibrary, (const Path& source));
        S_METHOD_DEF(Unit, AddDefine, (const std::string& source));
        S_METHOD_DEF(Unit, DeleteDefine, (const std::string& source));

    private:
        Scope<CompilerImpl> impl_;
    };
}  // namespace DummyEngine