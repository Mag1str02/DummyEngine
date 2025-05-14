#ifndef DUMMYENGINE_AUDIOCONTEXT_H
#define DUMMYENGINE_AUDIOCONTEXT_H
#include "AudioEngineFacade.h"
#include "AudioFile.h"
#include "DummyEngine/Utils/Base.h"

namespace DE {
    class AudioContext {
    public:
        AudioContext();
        ~AudioContext();

        AudioContext& operator= (const AudioContext&) = delete;
        AudioContext(const AudioContext&) = delete;

        AudioContext(AudioContext&& other) noexcept;
        AudioContext& operator=(AudioContext&& other) noexcept;

    private:
        ALCdevice* openALDevice;
        ALCcontext* openALContext;
        ALCboolean isMadeCurrent;
    };
}

#endif  // DUMMYENGINE_AUDIOCONTEXT_H
