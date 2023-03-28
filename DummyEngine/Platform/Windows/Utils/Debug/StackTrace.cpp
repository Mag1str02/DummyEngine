
#include "DummyEngine/Utils/Debug/StackTrace.h"

#include "DummyEngine/Utils/Debug/Demangler.h"

// clang-format off
#include <windows.h>
#include <dbghelp.h>
#include <intrin.h>
// clang-format on

namespace DE {
    struct StackFrame {
        DWORD64      address;
        std::string  name;
        std::string  module;
        unsigned int line;
        std::string  file;
    };

    std::string basename(const std::string& file) {
        size_t i = file.find_last_of("\\/");
        if (i == std::string::npos) {
            return file;
        } else {
            return file.substr(i + 1);
        }
    }

    std::string StackTrace() {
#if 1
        return "StackTrace not impemented\n";
#else
        std::stringstream ss;
        DWORD             machine = IMAGE_FILE_MACHINE_AMD64;
        HANDLE            process = GetCurrentProcess();
        HANDLE            thread  = GetCurrentThread();

        if (SymInitialize(process, NULL, TRUE) == FALSE) {
            ss << "Failed to get stack trace\n";
            return ss.str();
        }
        SymSetOptions(SYMOPT_LOAD_LINES);

        CONTEXT context      = {};
        context.ContextFlags = CONTEXT_FULL;
        RtlCaptureContext(&context);

        STACKFRAME frame       = {};
        frame.AddrPC.Offset    = context.Rip;
        frame.AddrPC.Mode      = AddrModeFlat;
        frame.AddrFrame.Offset = context.Rbp;
        frame.AddrFrame.Mode   = AddrModeFlat;
        frame.AddrStack.Offset = context.Rsp;
        frame.AddrStack.Mode   = AddrModeFlat;
        bool first             = true;

        std::vector<StackFrame> frames;
        while (StackWalk(machine, process, thread, &frame, &context, NULL, SymFunctionTableAccess, SymGetModuleBase, NULL)) {
            StackFrame f       = {};
            f.address          = frame.AddrPC.Offset;
            DWORD64 moduleBase = 0;

            moduleBase = SymGetModuleBase(process, frame.AddrPC.Offset);

            char moduelBuff[MAX_PATH];
            if (moduleBase && GetModuleFileNameA((HINSTANCE)moduleBase, moduelBuff, MAX_PATH)) {
                f.module = basename(moduelBuff);
            } else {
                f.module = "Unknown Module";
            }

            DWORD64 offset = 0;

            char             symbolBuffer[sizeof(IMAGEHLP_SYMBOL) + 255];
            PIMAGEHLP_SYMBOL symbol = (PIMAGEHLP_SYMBOL)symbolBuffer;
            symbol->SizeOfStruct    = sizeof(IMAGEHLP_SYMBOL) + 255;
            symbol->MaxNameLength   = 254;

            if (SymGetSymFromAddr(process, frame.AddrPC.Offset, &offset, symbol)) {
                f.name = symbol->Name;
                int         status;
                std::string demangled = DemangledName(f.name);
                if (demangled.empty()) {
                    f.name = symbol->Name;
                } else {
                    f.name = demangled;
                }
            } else {
                f.name = "Unknown Function";
            }

            IMAGEHLP_LINE line;
            line.SizeOfStruct = sizeof(IMAGEHLP_LINE);

            DWORD offset_ln = 0;
            if (SymGetLineFromAddr(process, frame.AddrPC.Offset, &offset_ln, &line)) {
                f.file = line.FileName;
                f.line = line.LineNumber;
            } else {
                f.line = 0;
            }

            if (!first) {
                frames.push_back(f);
            }
            first = false;
        }
        SymCleanup(process);

        for (const auto& frame : frames) {
            ss << frame.name << " in " << frame.module << "\n";
        }

        return ss.str();
#endif
    }
}  // namespace DE