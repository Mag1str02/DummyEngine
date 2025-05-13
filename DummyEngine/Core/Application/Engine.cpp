#include "Engine.h"

#include "Application.h"
#include "Initializer.h"

namespace DummyEngine {

    class Engine {
    public:
        int Run(FSetupApplication setup) {
            try {
                DummyEngine::Initializer::Initialize();

                setup();
                DummyEngine::Application::Run();

                DummyEngine::Initializer::Terminate();
            } catch (const std::exception& e) {
                LOG_FATAL_AS("EntryPoint", "Unhandled exeption occured: {}", e.what());
                return -1;
            } catch (...) {
                LOG_FATAL_AS("EntryPoint", "Unknown error occured");
                return -1;
            }
            return 0;
        }
    };

    int Main(FSetupApplication setup) {
        Engine engine;
        return engine.Run(setup);
    }

}  // namespace DummyEngine
