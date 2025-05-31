
#include <tracy/Tracy.hpp>

#include <DummyEngine/Utils/Debug/Assert.h>
#include <DummyEngine/Utils/PCH/DC.h>
#include <DummyConcurrency/DummyConcurrency.hpp>

using namespace DummyEngine;
using namespace std::chrono_literals;

const char* fiber = "SOME";

int main() {
    tracy::StartupProfiler();
    RunLoop loop;

    Fibers::Go(  //
        loop,
        [&]() {
            // TracyFiberEnter(fiber);
            for (int i = 0; i < 100000; ++i) {
                DE_PROFILE_SCOPE("Scope stuff");
                Submit(loop, []() { DE_PROFILE_SCOPE("Task scope"); });
                // TracyFiberLeave;
                Fibers::Yield();
                // TracyFiberEnter(fiber);
            }
            // TracyFiberLeave;
        },
        {.Name = "SOME", .Group = 0});
    loop.Run();
    std::this_thread::sleep_for(15s);
    tracy::ShutdownProfiler();
}
