#ifndef DUMMYENGINE_SOUNDSANDFACTORIES_H
#define DUMMYENGINE_SOUNDSANDFACTORIES_H
#pragma once

#include <utility>

#include "AudioEngineFacade.h"
#include "AudioFile.h"
#include "WAVLoader.h"
#include "DummyEngine/Utils/Base.h"

namespace DE {
    class Buffer {
    public:
        virtual ~Buffer() = default;

        Buffer(const Buffer&) = delete;
        Buffer& operator=(const Buffer&) = delete;

        Buffer(Buffer&& other) noexcept : bufferID(other.bufferID) { other.bufferID = 0; }
        Buffer& operator=(Buffer&& other) noexcept {
            if (this != &other) {
                alDeleteBuffers(1, &bufferID);
                bufferID = other.bufferID;
                other.bufferID = 0;
            }
            return *this;
        }

        ALuint getBufferID() const { return bufferID; }

    protected:
        Buffer() : bufferID(0) { alGenBuffers(1, &bufferID); }

        ALuint bufferID;
    };

    class RegularBuffer : public Buffer {
    public:
        RegularBuffer();
        ~RegularBuffer();
    };

    class StreamingBuffer : public Buffer {
    public:
        static constexpr size_t NUM_BUFFERS = 3;

        StreamingBuffer();
        ~StreamingBuffer();

    private:
        ALuint buffers[NUM_BUFFERS];
    };

    class BufferFactory {
    public:
        BufferFactory() = default;
        ~BufferFactory() = default;

        std::unique_ptr<Buffer> createBuffer();

        friend class SoundFactory;
    };

    class Sound {
    public:
        Sound() = default;
        Sound(std::string filepath) : filepath(std::move(filepath)) {}
        virtual ~Sound() = default;

        Sound(const Sound&) = delete;
        Sound& operator=(const Sound&) = delete;
        Sound(Sound&&) = default;
        Sound& operator=(Sound&&) = default;

        virtual void init() = 0;
        virtual void init_streaming() = 0;
        virtual void play() = 0;
        virtual void stop() = 0;

    protected:
        std::string filepath;
        std::unique_ptr<Buffer> buffer;
        ALenum format;
        ALuint source;
        ALint state;
    };

    class WavSound : public Sound {
    public:
        WavSound() = default;
        ~WavSound();
        explicit WavSound(const std::string& filepath) : Sound(filepath) {}

        void init() override; // open file;

        void init_streaming() override;

        void play() override;

        void stop() override;
    private:
        // AudioFile<int32_t> audioFile; // using external library to work with .WAV

    };

    class SoundFactory {
    public:
        enum class SoundType {
            WAV,
        };

        SoundFactory() = default;
        ~SoundFactory() = default;

        std::unique_ptr<Sound> createSound(SoundType type, const std::string& filepath="");
    private:
        BufferFactory bufferFactory;
    };

}  // namespace DE

#endif  // DUMMYENGINE_SOUNDSANDFACTORIES_H
