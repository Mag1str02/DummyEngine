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

    StreamingBuffer::StreamingBuffer() {
        alCall(alGenBuffers, NUM_BUFFERS, &buffers[0]);
    }

    StreamingBuffer::~StreamingBuffer() {
        alCall(alDeleteBuffers, NUM_BUFFERS, &buffers[0]);
    }

    void StreamingBuffer::update_stream(const ALuint source, const ALenum& format, const std::int32_t& sampleRate, const std::vector<char>& soundData, std::size_t& cursor) {
        ALint buffersProcessed = 0;
        alCall(alGetSourcei, source, AL_BUFFERS_PROCESSED, &buffersProcessed);

        if(buffersProcessed <= 0)
            return;

        while(buffersProcessed--)
        {
            ALuint buffer;
            alCall(alSourceUnqueueBuffers, source, 1, &buffer);

            ALsizei dataSize = BUFFER_SIZE;

            char* data = new char[dataSize];
            std::memset(data, 0, dataSize);

            std::size_t dataSizeToCopy = BUFFER_SIZE;
            if(cursor + BUFFER_SIZE > soundData.size())
                dataSizeToCopy = soundData.size() - cursor;

            std::memcpy(&data[0], &soundData[cursor], dataSizeToCopy);
            cursor += dataSizeToCopy;

            if(dataSizeToCopy < BUFFER_SIZE)
            {
                cursor = 0;
                std::memcpy(&data[dataSizeToCopy], &soundData[cursor], BUFFER_SIZE - dataSizeToCopy);
                cursor = BUFFER_SIZE - dataSizeToCopy;
            }

            alCall(alBufferData, buffer, format, data, BUFFER_SIZE, sampleRate);
            alCall(alSourceQueueBuffers, source, 1, &buffer);

            delete[] data;
        }
    }

    void WavSound::init() {
        soundData = load_wav(filepath, channels, sampleRate, bitsPerSample, size);

        buffer = std::make_unique<RegularBuffer>();

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

    ALuint StreamingBuffer::get_buffer(size_t i) {
        if (i >= NUM_BUFFERS) {
            return -1;
        }
        return buffers[i];
    }

    ALuint* StreamingBuffer::get_buffer_pointer(size_t i) {
        if (i >= NUM_BUFFERS) {
            return nullptr;
        }
        return &buffers[i];
    }

    void WavSound::init_streaming() {
        soundData = load_wav(filepath, channels, sampleRate, bitsPerSample, size);

        buffer = std::make_unique<StreamingBuffer>();

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

        for(std::size_t i = 0; i < StreamingBuffer::NUM_BUFFERS; ++i) {
            alCall(alBufferData, reinterpret_cast<StreamingBuffer*>(buffer.get())->get_buffer(i), format, &soundData[i * StreamingBuffer::BUFFER_SIZE], StreamingBuffer::BUFFER_SIZE, sampleRate);
        }

        alCall(alGenSources, 1, &source);
        alCall(alSourcef, source, AL_PITCH, 1);
        alCall(alSourcef, source, AL_GAIN, 1.0f);
        alCall(alSource3f, source, AL_POSITION, 0, 0, 0);
        alCall(alSource3f, source, AL_VELOCITY, 0, 0, 0);
        alCall(alSourcei, source, AL_LOOPING, AL_FALSE);
    }

    ALfloat Sound::getGain() {
        ALfloat gain;
        alCall(alGetSourcef, source, AL_GAIN, &gain);
        return gain;
    }

    void Sound::setGain(ALfloat gain) {
        alCall(alSourcef, source, AL_GAIN, gain);
    }

    ALfloat Sound::getPitch() {
        ALfloat pitch;
        alCall(alGetSourcef, source, AL_PITCH, &pitch);
        return pitch;
    }

    void Sound::setPitch(ALfloat pitch) {
        alCall(alSourcef, source, AL_PITCH, pitch);
    }

    std::vector<ALfloat> Sound::getPosition() {
        std::vector<ALfloat> position(3);
        alCall(alGetSource3f, source, AL_POSITION, &position[0], &position[1], &position[2]);
        return position;
    }

    void Sound::setPosition(std::vector<ALfloat> position) {
        if (position.size() != 3) {
            throw std::runtime_error("Invalid position vector size");
        }
        alCall(alSource3f, source, AL_POSITION, position[0], position[1], position[2]);
    }

    std::vector<ALfloat> Sound::getVelocity() {
        std::vector<ALfloat> velocity(3);
        alCall(alGetSource3f, source, AL_VELOCITY, &velocity[0], &velocity[1], &velocity[2]);
        return velocity;
    }

    void Sound::setVelocity(std::vector<ALfloat> velocity) {
        if (velocity.size() != 3) {
            throw std::runtime_error("Invalid velocity vector size");
        }
        alCall(alSource3f, source, AL_VELOCITY, velocity[0], velocity[1], velocity[2]);
    }

    ALboolean Sound::getLooping() {
        ALint looping;
        alCall(alGetSourcei, source, AL_LOOPING, &looping);
        return looping;
    }

    void Sound::setLooping(ALboolean looping) {
        alCall(alSourcei, source, AL_LOOPING, looping);
    }

    WavSound::~WavSound() {
        alCall(alDeleteSources, 1, &source);
    }

    void WavSound::play() {
        alCall(alSourcePlay, source);

        state = AL_PLAYING;
        alCall(alGetSourcei, source, AL_SOURCE_STATE, &state);
    }

    void WavSound::play_streaming() {
        alCall(alSourceQueueBuffers, source, StreamingBuffer::NUM_BUFFERS, (reinterpret_cast<StreamingBuffer*>(buffer.get())->get_buffer_pointer(0)));
        alCall(alSourcePlay, source);

        ALint state = AL_PLAYING;

        std::size_t cursor = StreamingBuffer::BUFFER_SIZE * StreamingBuffer::NUM_BUFFERS;

        while(state == AL_PLAYING)
        {
            reinterpret_cast<StreamingBuffer*>(buffer.get())->update_stream(source, format, sampleRate, soundData, cursor);
            alCall(alGetSourcei, source, AL_SOURCE_STATE, &state);
        }
    }

    void WavSound::stop() {
        alCall(alSourceStop, source);
        state = AL_STOPPED;
    }

    void WavSound::stop_streaming() {
        alCall(alSourceStop, source);
        ALint queued;
        alCall(alGetSourcei, source, AL_BUFFERS_QUEUED, &queued);

        while (queued--) {
            ALuint buffer;
            alCall(alSourceUnqueueBuffers, source, 1, &buffer);
        }
    }

}  // namespace DE