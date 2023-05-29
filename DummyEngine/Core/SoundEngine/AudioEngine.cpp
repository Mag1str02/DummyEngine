#include "AudioEngine.h"

namespace DE {
    SINGLETON_BASE(AudioEngine);
    S_INITIALIZE() {
        openAL_device = alcOpenDevice(nullptr);

        ALCcontext* tmp;
        if (!alcCall(alcCreateContext, tmp, openAL_device, openAL_device, nullptr)) {
            std::cerr << "Couldn't create audio context";
        }
        openAL_context = tmp;

        is_made_current = false;
        if (!alcCall(alcMakeContextCurrent, is_made_current, openAL_device, openAL_context) || is_made_current != ALC_TRUE) {
            std::cerr << "Couldn't make context current";
        }
        return Unit();
    }

    S_TERMINATE() {
        if(!alcCall(alcMakeContextCurrent, is_made_current, openAL_device, nullptr))
        {
            /* what can you do? */
        }

        if(!alcCall(alcDestroyContext, openAL_device, openAL_context))
        {
            /* not much you can do */
        }
        return Unit();
    }
}