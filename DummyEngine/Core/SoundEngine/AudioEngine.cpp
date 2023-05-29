#include "AudioEngine.h"

namespace DE {
    SINGLETON_BASE(AudioEngine);
    S_INITIALIZE() {
        openAL_device = alcOpenDevice(nullptr);

//        openAL_context = alcCreateContext(openAL_device, nullptr);
        if(!alcCall(alcCreateContext, openAL_context, openAL_device, openAL_device, nullptr) || !openAL_context)
        {
            std::cerr << "ERROR: Could not create audio context" << std::endl;

        }

        is_made_current = false;
//        is_made_current = alcMakeContextCurrent(openAL_context);
        if (!alcCall(alcMakeContextCurrent, is_made_current, openAL_device, openAL_context) || is_made_current != ALC_TRUE) {
            std::cerr << "Couldn't make context current";
        }
        return Unit();
    }

    S_TERMINATE() {
        if(!alcCall(alcMakeContextCurrent, is_made_current, openAL_device, nullptr))
        {

        }
//        is_made_current = alcMakeContextCurrent(nullptr);


        if(!alcCall(alcDestroyContext, openAL_device, openAL_context))
        {

        }
//        alcDestroyContext(openAL_context);
        return Unit();
    }
}