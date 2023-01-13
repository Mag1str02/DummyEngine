#include "DummyEngine/Core/Scripting/Compiler.h"

namespace DE
{
    class CompilerImpl
    {
    public:
        bool Compile(const Path& source, const Path& destination)
        {
            if (!fs::is_directory(destination))
            {
                return false;
            }
            if (!fs::exists(source))
            {
                return false;
            }

            std::string compile_command;
            compile_command.append(GetCompiler());
            compile_command.append(" -c");
            compile_command.append(AddIncludeDirArguments());
            compile_command.append(AddSourceArgument(source));
            compile_command.append(AddDestinationArgument(source, destination));
            FixSlash(compile_command);

            int res = system(compile_command.c_str());

            return res == 0;
        }
        bool Link(const std::vector<Path>& sources, const Path& destination, const std::string& library_name)
        {
            for (const auto& source : sources)
            {
                if (!fs::exists(source))
                {
                    return false;
                }
            }
            if (!fs::is_directory(destination))
            {
                return false;
            }

            std::string link_command;
            link_command.append(GetCompiler());
            link_command.append(" -shared");
            link_command.append(AddSourcesArguments(sources));
            link_command.append(AddPathDLL(destination, library_name));
            FixSlash(link_command);
            int res = system(link_command.c_str());

            return res == 0;
        }
        bool AddIncludeDir(const Path& dir)
        {
            if (!fs::is_directory(dir))
            {
                return false;
            }
            m_IncludeDirs.insert(dir);
            return true;
        }
        void DeleteIncludeDir(const Path& dir) { m_IncludeDirs.erase(dir); }

    private:
        std::string AddSourcesArguments(const std::vector<Path>& sources)
        {
            std::string res;
            for (const auto& source : sources)
            {
                res.append(" ");
                res.append(source.string());
            }
            return res;
        }
        std::string AddPathDLL(const Path& destination, const std::string& library_name)
        {
            return " -o " + destination.string() + "/" + library_name + ".dll";
        }
        std::string AddIncludeDirArguments()
        {
            std::string res;
            for (const auto& dir : m_IncludeDirs)
            {
                res.append(" -I ");
                res.append(dir.string());
            }
            return res;
        }
        std::string GetCompiler()
        {
            // TODO: Get compiler from config.
            return "g++";
        }
        std::string AddSourceArgument(const Path& source) { return " " + source.string(); }
        std::string AddDestinationArgument(const Path& source, const Path& destination)
        {
            return " -o " + destination.string() + "/" + source.filename().string() + ".o";
        }
        void FixSlash(std::string& command) { std::replace(command.begin(), command.end(), '\\', '/'); }

        std::unordered_set<Path> m_IncludeDirs;
    };

    Scope<CompilerImpl> Compiler::m_Impl;

    void Compiler::StartUp()
    {
        DE_ASSERT(!m_Impl, "Compiler already started.");
        m_Impl = CreateScope<CompilerImpl>();
    }
    void Compiler::ShutDown() {}

    bool Compiler::Compile(const Path& source, const Path& destination) { return m_Impl->Compile(source, destination); }
    bool Compiler::AddIncludeDir(const Path& dir) { return m_Impl->AddIncludeDir(dir); }
    void Compiler::DeleteIncludeDir(const Path& dir) { m_Impl->DeleteIncludeDir(dir); }

};  // namespace DE

//* g++ --shared Assets/Scripts/DummyEditor/EditorCameraController.cpp -I ../src -I ../src/DummyEngine/Libs/GLM -L . -l DummyEngineLib -o
// E:/IT_Space/Programs/C++/OpenGL/DummyEngine/Sandbox/Cache/Scripts/DummyEditor/EditorCameraController.dll