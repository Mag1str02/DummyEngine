#ifndef DUMMYENGINE_AUDIOENGINE_H
#define DUMMYENGINE_AUDIOENGINE_H
#include "AudioEngineFacade.h"
#include "AudioFile.h"
#include "DummyEngine/Utils/Base.h"

namespace DE {
    class AudioEngine : public Singleton<AudioEngine>{
        SINGLETON(AudioEngine)
    public:

    private:
        ALCdevice* openAL_device;
        ALCcontext* openAL_context;
        ALCboolean is_made_current;
    };
}

#endif  // DUMMYENGINE_AUDIOENGINE_H
