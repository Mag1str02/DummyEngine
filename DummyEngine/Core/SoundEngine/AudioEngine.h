#ifndef DUMMYENGINE_AUDIOENGINE_H
#define DUMMYENGINE_AUDIOENGINE_H
#pragma once

#include "AudioContext.h"
#include "AudioEngineFacade.h"
#include "AudioFile.h"
#include "DummyEngine/Utils/Base.h"
#include "SoundsAndFactories.h"

namespace DE {
    class AudioEngine : public Singleton<AudioEngine> {
        SINGLETON(AudioEngine)
    public:
    private:
        AudioContext main_context;
        SoundFactory soundFactory{};
    };
}  // namespace DE

#endif  // DUMMYENGINE_AUDIOENGINE_H
