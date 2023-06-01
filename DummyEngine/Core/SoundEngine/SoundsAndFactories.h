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
                alCall(alDeleteBuffers, 1, &bufferID);
                bufferID = other.bufferID;
                filename = other.filename;
                other.bufferID = 0;
            }
            return *this;
        }

        ALuint getBufferID() const { return bufferID; }

    protected:
        Buffer() : bufferID(0) { alGenBuffers(1, &bufferID); }

        ALuint bufferID;
        std::string filename;
    };

    class RegularBuffer : public Buffer {
    public:
        RegularBuffer();
        RegularBuffer(const std::string& filename);
        ~RegularBuffer();
    };

    class StreamingBuffer : public Buffer {
    public:
        static constexpr size_t NUM_BUFFERS = 4;
        static constexpr size_t BUFFER_SIZE = 65536;

        StreamingBuffer();
        StreamingBuffer(const std::string& filename);
        ~StreamingBuffer();

        void update_stream(const ALuint source,
                           const ALenum& format,
                           const std::int32_t& sampleRate,
                           const std::vector<char>& soundData,
                           std::size_t& cursor);

        ALuint get_buffer(size_t i);

        ALuint* get_buffer_pointer(size_t i);

    private:
        ALuint buffers[NUM_BUFFERS];
    };

    class BufferFactory {
    public:
        enum class BufferType {
            REGULAR,
            STREAMING,
        };

        BufferFactory() = default;
        ~BufferFactory() = default;

        std::unique_ptr<Buffer> createBuffer(BufferFactory::BufferType, const std::string& filepath);

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
        virtual void play_streaming() = 0;
        virtual void stop() = 0;
        virtual void stop_streaming() = 0;

        virtual ALfloat getGain();
        virtual void setGain(ALfloat gain);

        ALfloat getPitch();
        void setPitch(ALfloat pitch);

        std::vector<ALfloat> getPosition();
        void setPosition(std::vector<ALfloat> position);

        std::vector<ALfloat> getVelocity();
        void setVelocity(std::vector<ALfloat> velocity);

        ALboolean getLooping();
        void setLooping(ALboolean looping);

    protected:
        std::string filepath;
        std::unique_ptr<Buffer> buffer;
        ALuint source;
        ALenum format;
        ALint state;

        std::uint8_t channels;
        std::int32_t sampleRate;
        std::uint8_t bitsPerSample;
        ALsizei size;
        std::vector<char> soundData;
    };

    class WavSound : public Sound {
    public:
        WavSound() = default;
        ~WavSound() override;
        explicit WavSound(const std::string& filepath) : Sound(filepath) {}

        void init() override; // open file;

        void init_streaming() override;

        void play() override;

        void play_streaming() override;

        void stop() override;

        void stop_streaming() override;
    private:
        // AudioFile<int32_t> audioFile;
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
