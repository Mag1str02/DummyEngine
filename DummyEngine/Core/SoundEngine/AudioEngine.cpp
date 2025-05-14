#include "AudioEngine.h"

namespace DE {
    SINGLETON_BASE(AudioEngine);
    S_INITIALIZE() {
        return Unit();
    }

    S_TERMINATE() {
        return Unit();
    }
}