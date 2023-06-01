#include "SoundsAndFactories.h"

namespace DE {
    std::unique_ptr<Sound> SoundFactory::createSound(DE::SoundFactory::SoundType type, const std::string& filepath)  {
        switch (type) {
            case SoundType::WAV:
                return std::make_unique<WavSound>(filepath);
            default:
                throw std::invalid_argument("Invalid sound type");
        }
    }

    std::unique_ptr<Buffer> BufferFactory::createBuffer(BufferFactory::BufferType type, const std::string& filepath) {
        switch (type) {
            case BufferType::REGULAR:

            case BufferType::STREAMING:

            default:
                throw std::invalid_argument("Invalid buffer type");
        }
    }

    RegularBuffer::RegularBuffer() {
        alCall(alGenBuffers, 1, &bufferID);
    }

    RegularBuffer::RegularBuffer(const std::string& filename) {
        std::uint8_t channels;
        std::int32_t sampleRate;
        std::uint8_t bitsPerSample;
        ALsizei size;
        std::vector<char> soundData = load_wav(filename, channels, sampleRate, bitsPerSample, size);

        ALenum format;
        if (channels == 1 && bitsPerSample == 8) {
            format = AL_FORMAT_MONO8;
        }
        else if (channels == 1 && bitsPerSample == 16) {
            format = AL_FORMAT_MONO16;
        }
        else if (channels == 2 && bitsPerSample == 8) {
            format = AL_FORMAT_STEREO8;
        }
        else if (channels == 2 && bitsPerSample == 16) {
            format = AL_FORMAT_STEREO16;
        }
        else
        {
            std::cerr
                << "ERROR: unrecognised wave format: "
                << channels << " channels, "
                << bitsPerSample << " bps" << std::endl;
        }

        alCall(alBufferData, bufferID, format, soundData.data(), soundData.size(), sampleRate);
        soundData.clear(); // erase the sound in RAM
    }

    RegularBuffer::~RegularBuffer() {
        alCall(alDeleteBuffers, 1, &bufferID);
    }

    void WavSound::init() {
        std::uint8_t channels;
        std::int32_t sampleRate;
        std::uint8_t bitsPerSample;
        ALsizei size;
        std::vector<char> soundData = load_wav(filepath, channels, sampleRate, bitsPerSample, size);

        buffer = std::make_unique<RegularBuffer>();
        ALenum format;

        if (channels == 1 && bitsPerSample == 8) {
            format = AL_FORMAT_MONO8;
        }
        else if (channels == 1 && bitsPerSample == 16) {
            format = AL_FORMAT_MONO16;
        }
        else if (channels == 2 && bitsPerSample == 8) {
            format = AL_FORMAT_STEREO8;
        }
        else if (channels == 2 && bitsPerSample == 16) {
            format = AL_FORMAT_STEREO16;
        }
        else
        {
            std::cerr
                << "ERROR: unrecognised wave format: "
                << channels << " channels, "
                << bitsPerSample << " bps" << std::endl;
        }

        alCall(alBufferData, buffer->getBufferID(), format, soundData.data(), soundData.size(), sampleRate);
        soundData.clear(); // erase the sound in RAM

        alCall(alGenSources, 1, &source);
        alCall(alSourcef, source, AL_PITCH, 1);
        alCall(alSourcef, source, AL_GAIN, 1.0f);
        alCall(alSource3f, source, AL_POSITION, 0, 0, 0);
        alCall(alSource3f, source, AL_VELOCITY, 0, 0, 0);
        alCall(alSourcei, source, AL_LOOPING, AL_FALSE);
        alCall(alSourcei, source, AL_BUFFER, buffer->getBufferID());
    }

    void WavSound::init_streaming() {

    }

    WavSound::~WavSound() {
        alCall(alDeleteSources, 1, &source);
    }

    void WavSound::play() {
        alCall(alSourcePlay, source);

        state = AL_PLAYING;
        alCall(alGetSourcei, source, AL_SOURCE_STATE, &state);
    }

    void WavSound::stop() {
        alCall(alSourceStop, source);
        state = AL_STOPPED;
    }

}  // namespace DE