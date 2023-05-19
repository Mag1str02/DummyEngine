#include "DummyEditor/Scripting/Compiler.h"

namespace DE {
    class CompilerImpl {
    public:
        bool Compile(const Path& source, const Path& destination) {
            if (!fs::exists(source) || destination.filename().empty()) {
                LOG_ERROR("Compiler", "File (", source, ") was not compiled because does not exists or destination empty");
                return false;
            }

            Path directory = destination;
            directory.remove_filename();
            FileSystem::CreateDirectory(directory);

            std::string compile_command;
            compile_command.append(GetCompiler());
            compile_command.append(" -c -std=c++20");
            compile_command.append(AddDefines());
            compile_command.append(AddIncludeDirArguments());
            compile_command.append(AddSourceArgument(source));
            compile_command.append(AddDestinationArgument(source, destination));
            FixSlash(compile_command);

            int res = system(compile_command.c_str());

            LOG_INFO("Compiler", "Compiled file (", RelativeToExecutable(source), ")");
            return res == 0;
        }
        bool Link(const std::vector<Path>& sources, const Path& destination, const std::string& library_name) {
            if (library_name.empty()) {
                LOG_ERROR("Compiler", "Failed to link library because of empty library name");
                return false;
            }
            if (!fs::is_directory(destination)) {
                LOG_ERROR("Compiler", "Failed to link library because of wrong destination folder (", destination.string(), ")");
                return false;
            }
            for (const auto& source : sources) {
                if (!fs::exists(source)) {
                    LOG_ERROR("Compiler", "Failed to link library (", library_name, ") because of a missing source(", source, ")");
                    return false;
                }
            }
            FileSystem::CreateDirectory(destination);

            std::string link_command;
            link_command.append(GetCompiler());
            link_command.append(" -shared");
            link_command.append(AddLinkArgs());
            link_command.append(AddSourcesArguments(sources));
            link_command.append(AddPathDLL(destination, library_name));
            FixSlash(link_command);

            int res = system(link_command.c_str());
            if (res != 0) {
                LOG_ERROR("Compiler", "Linking of (", library_name, ") failed with code (", res, ")");
            } else {
                LOG_INFO("Compiler", "Linked (", library_name, ")");
            }
            return res == 0;
        }
        void AddIncludeDir(const Path& dir) { m_IncludeDirs.insert(dir); }
        void DeleteIncludeDir(const Path& dir) { m_IncludeDirs.erase(dir); }
        void AddLinkLibrary(const Path& library) { m_Libraries.insert(library); }
        void DeleteLinkLibrary(const Path& library) { m_Libraries.erase(library); }
        void AddDefine(const std::string& define) { m_Defines.insert(define); }
        void DeleteDefine(const std::string& define) { m_Defines.erase(define); }

    private:
        std::string GetCompiler() {
            // TODO: Get compiler from config.
            return "g++";
        }
        void FixSlash(std::string& command) { std::replace(command.begin(), command.end(), '\\', '/'); }

        std::string AddIncludeDirArguments() {
            std::string res;
            for (const auto& dir : m_IncludeDirs) {
                res.append(" -I ");
                res.append(dir.string());
            }
            return res;
        }
        std::string AddSourceArgument(const Path& source) { return " " + source.string(); }
        std::string AddDestinationArgument(const Path& source, const Path& destination) { return " -o " + destination.string(); }

        std::string AddSourcesArguments(const std::vector<Path>& sources) {
            std::string res;
            for (const auto& source : sources) {
                res.append(" ");
                res.append(source.string());
            }
            return res;
        }
        std::string AddLinkArgs() {
            std::string res;
            for (auto lib : m_Libraries) {
                std::string name = lib.stem().string();
                lib.remove_filename();
                if (lib.empty()) {
                    lib = ".";
                }
                res.append(" -L ");
                res.append(lib.string());
                res.append(" -l ");
                res.append(name);
            }
            return res;
        }
        std::string AddDefines() {
            std::string res;
            for (const auto& def : m_Defines) {
                res.append(" -D");
                res.append(def);
            }
            return res;
        }
        std::string AddPathDLL(const Path& destination, const std::string& library_name) {
            return " -o " + destination.string() + "/" + library_name + ".dll";
        }

        std::unordered_set<Path>        m_IncludeDirs;
        std::unordered_set<Path>        m_Libraries;
        std::unordered_set<std::string> m_Defines;
    };

    Scope<CompilerImpl> Compiler::m_Impl;

    void Compiler::Initialize() {
        DE_ASSERT(!m_Impl, "Compiler already started.");
        m_Impl = CreateScope<CompilerImpl>();
        AddIncludeDir("..");
        AddIncludeDir("../DummyEngine/Libs/GLM");
        AddLinkLibrary("DummyEngineLib");
        AddDefine("DE_PLATFORM_WINDOWS");
        AddDefine("DE_ENABLE_LOGGING=" + std::to_string(DE_ENABLE_LOGGING));
        AddDefine("DE_ENABLE_ASSERTS=" + std::to_string(DE_ENABLE_ASSERTS));
        AddDefine("DE_ENABLE_PROFILER=" + std::to_string(DE_ENABLE_PROFILER));
    }
    void Compiler::Terminate() {
        m_Impl = nullptr;
    }

    bool Compiler::Compile(const Path& source, const Path& destination) {
        return m_Impl->Compile(source, destination);
    }
    bool Compiler::Link(const std::vector<Path>& sources, const Path& destination, const std::string& library_name) {
        return m_Impl->Link(sources, destination, library_name);
    }
    void Compiler::AddIncludeDir(const Path& dir) {
        m_Impl->AddIncludeDir(dir);
    }
    void Compiler::DeleteIncludeDir(const Path& dir) {
        m_Impl->DeleteIncludeDir(dir);
    }
    void Compiler::AddLinkLibrary(const Path& library) {
        m_Impl->AddLinkLibrary(library);
    }
    void Compiler::DeleteLinkLibrary(const Path& library) {
        m_Impl->DeleteLinkLibrary(library);
    }
    void Compiler::AddDefine(const std::string& source) {
        m_Impl->AddDefine(source);
    }
    void Compiler::DeleteDefine(const std::string& source) {
        m_Impl->DeleteDefine(source);
    }

};  // namespace DE