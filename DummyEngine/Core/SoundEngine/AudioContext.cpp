#include "AudioContext.h"

namespace DE {
    AudioContext::AudioContext() {
        openALDevice  = alcOpenDevice(nullptr);
        openALContext = nullptr;
        if (!alcCall(alcCreateContext, openALContext, openALDevice, openALDevice, nullptr) || !openALContext) {
            std::cerr << "Could not create audio context" << std::endl;
        }

        isMadeCurrent = false;
        if (!alcCall(alcMakeContextCurrent, isMadeCurrent, openALDevice, openALContext) || isMadeCurrent != ALC_TRUE) {
            std::cerr << "Couldn't make context current";
        }
    }

    AudioContext::~AudioContext() {
        if (!alcCall(alcMakeContextCurrent, isMadeCurrent, openALDevice, nullptr)) {
        }

        if (!alcCall(alcDestroyContext, openALDevice, openALContext)) {
        }

        ALCboolean closed;
        alcCall(alcCloseDevice, closed, openALDevice, openALDevice);
    }

    AudioContext::AudioContext(AudioContext&& other) noexcept :
        openALDevice(other.openALDevice), openALContext(other.openALContext), isMadeCurrent(other.isMadeCurrent) {
        other.openALDevice  = nullptr;
        other.openALContext = nullptr;
        other.isMadeCurrent = ALC_FALSE;
    }

    AudioContext& AudioContext::operator=(AudioContext&& other) noexcept {
        if (this != &other) {
            // Free the existing resources
            alcMakeContextCurrent(nullptr);
            if (isMadeCurrent && openALContext != nullptr) {
                alcDestroyContext(openALContext);
            }

            if (openALDevice != nullptr) {
                alcCloseDevice(openALDevice);
            }

            // Move the resources from the other instance
            openALDevice  = other.openALDevice;
            openALContext = other.openALContext;
            isMadeCurrent = other.isMadeCurrent;

            // Null out the resources in the other instance
            other.openALDevice  = nullptr;
            other.openALContext = nullptr;
            other.isMadeCurrent = ALC_FALSE;
        }

        return *this;
    }
}  // namespace DE