#ifndef DUMMYENGINE_AUDIOENGINE_H
#define DUMMYENGINE_AUDIOENGINE_H
#pragma once

#include "AudioEngineFacade.h"
#include "AudioFile.h"
#include "DummyEngine/Utils/Base.h"
#include "AudioContext.h"
#include "SoundsAndFactories.h"

namespace DE {
    class AudioEngine : public Singleton <AudioEngine> {
        SINGLETON(AudioEngine)
    public:

    private:
        AudioContext main_context;
        SoundFactory soundFactory{};
    };
}

#endif  // DUMMYENGINE_AUDIOENGINE_H
