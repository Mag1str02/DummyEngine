#include "Engine.h"

#include "Application.h"
#include "Concurrency.h"
#include "GLFW.h"
#include "Initializer.h"

namespace DummyEngine {

    class Engine {
    public:
        explicit Engine(FSetupApplication setup) : setup_function_(setup) {}
        int Run() {
            GLFW::Initialize();
            Concurrency::Initialize();

            auto code = Futures::Submit(Concurrency::GetEngineMainScheduler(), [this]() { return EngineMain(); });
            Concurrency::GetMainThreadScheduler().BecomeWorker();

            auto return_code = std::move(code) | Futures::Get();
            Concurrency::Terminate();
            GLFW::Terminate();
            return return_code;
        }

    private:
        int EngineMain() {
            try {
                DummyEngine::Initializer::Initialize();

                setup_function_();
                DummyEngine::Application::Run();

                DummyEngine::Initializer::Terminate();
                Concurrency::GetMainThreadScheduler().Stop();
            } catch (const std::exception& e) {
                LOG_FATAL_AS("EntryPoint", "Unhandled exeption occured: {}", e.what());
                return -1;
            } catch (...) {
                LOG_FATAL_AS("EntryPoint", "Unknown error occured");
                return -1;
            }
            return 0;
        }

    private:
        FSetupApplication setup_function_;
    };

    int Main(FSetupApplication setup) {
        Engine engine(setup);
        auto   code = engine.Run();
        std::println("Exiting DummyEngine::Main");
        return code;
    }

}  // namespace DummyEngine
