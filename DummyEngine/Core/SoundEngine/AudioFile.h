#ifndef DUMMYENGINE_AUDIOFILE_H
#define DUMMYENGINE_AUDIOFILE_H

#if defined(_MSC_VER)
#undef max
#undef min
#define NOMINMAX
#endif

#include <algorithm>
#include <cassert>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <limits>
#include <string>
#include <unordered_map>
#include <vector>

// disable some warnings on Windows
#if defined(_MSC_VER)
__pragma(warning(push)) __pragma(warning(disable : 4244)) __pragma(warning(disable : 4457)) __pragma(warning(disable : 4458))
    __pragma(warning(disable : 4389)) __pragma(warning(disable : 4996))
#elif defined(__GNUC__)
_Pragma("GCC diagnostic push") _Pragma("GCC diagnostic ignored \"-Wconversion\"") _Pragma("GCC diagnostic ignored \"-Wsign-compare\"")
    _Pragma("GCC diagnostic ignored \"-Wshadow\"")
#endif

    //=============================================================
    /** The different types of audio file, plus some other types to
     * indicate a failure to load a file, or that one hasn't been
     * loaded yet
     */
    enum class AudioFileFormat {
        Error,
        NotLoaded,
        Wave,
        Aiff
    };

//=============================================================
template <class T> class AudioFile {
public:
    //=============================================================
    typedef std::vector<std::vector<T>> AudioBuffer;

    //=============================================================
    /** Constructor */
    AudioFile();

    /** Constructor, using a given file path to load a file */
    AudioFile(std::string filePath);

    //=============================================================
    /** Loads an audio file from a given file path.
     * @Returns true if the file was successfully loaded
     */
    bool load(std::string filePath);

    /** Saves an audio file to a given file path.
     * @Returns true if the file was successfully saved
     */
    bool save(std::string filePath, AudioFileFormat format = AudioFileFormat::Wave);

    //=============================================================
    /** Loads an audio file from data in memory */
    bool loadFromMemory(std::vector<uint8_t>& fileData);

    //=============================================================
    /** @Returns the sample rate */
    uint32_t getSampleRate() const;

    /** @Returns the number of audio channels in the buffer */
    int getNumChannels() const;

    /** @Returns true if the audio file is mono */
    bool isMono() const;

    /** @Returns true if the audio file is stereo */
    bool isStereo() const;

    /** @Returns the bit depth of each sample */
    int getBitDepth() const;

    /** @Returns the number of samples per channel */
    int getNumSamplesPerChannel() const;

    /** @Returns the length in seconds of the audio file based on the number of samples and sample rate */
    double getLengthInSeconds() const;

    /** Prints a summary of the audio file to the console */
    void printSummary() const;

    //=============================================================

    /** Set the audio buffer for this AudioFile by copying samples from another buffer.
     * @Returns true if the buffer was copied successfully.
     */
    bool setAudioBuffer(AudioBuffer& newBuffer);

    /** Sets the audio buffer to a given number of channels and number of samples per channel. This will try to preserve
     * the existing audio, adding zeros to any new channels or new samples in a given channel.
     */
    void setAudioBufferSize(int numChannels, int numSamples);

    /** Sets the number of samples per channel in the audio buffer. This will try to preserve
     * the existing audio, adding zeros to new samples in a given channel if the number of samples is increased.
     */
    void setNumSamplesPerChannel(int numSamples);

    /** Sets the number of channels. New channels will have the correct number of samples and be initialised to zero */
    void setNumChannels(int numChannels);

    /** Sets the bit depth for the audio file. If you use the save() function, this bit depth rate will be used */
    void setBitDepth(int numBitsPerSample);

    /** Sets the sample rate for the audio file. If you use the save() function, this sample rate will be used */
    void setSampleRate(uint32_t newSampleRate);

    //=============================================================
    /** Sets whether the library should log error messages to the console. By default this is true */
    void shouldLogErrorsToConsole(bool logErrors);

    //=============================================================
    /** A vector of vectors holding the audio samples for the AudioFile. You can
     * access the samples by channel and then by sample index, i.e:
     *
     *      samples[channel][sampleIndex]
     */
    AudioBuffer samples;

    //=============================================================
    /** An optional iXML chunk that can be added to the AudioFile.
     */
    std::string iXMLChunk;

private:
    //=============================================================
    enum class Endianness { LittleEndian, BigEndian };

    //=============================================================
    AudioFileFormat determineAudioFileFormat(std::vector<uint8_t>& fileData);
    bool            decodeWaveFile(std::vector<uint8_t>& fileData);
    bool            decodeAiffFile(std::vector<uint8_t>& fileData);

    //=============================================================
    bool saveToWaveFile(std::string filePath);
    bool saveToAiffFile(std::string filePath);

    //=============================================================
    void clearAudioBuffer();

    //=============================================================
    int32_t fourBytesToInt(std::vector<uint8_t>& source, int startIndex, Endianness endianness = Endianness::LittleEndian);
    int16_t twoBytesToInt(std::vector<uint8_t>& source, int startIndex, Endianness endianness = Endianness::LittleEndian);
    int     getIndexOfString(std::vector<uint8_t>& source, std::string s);
    int     getIndexOfChunk(std::vector<uint8_t>& source,
                            const std::string&    chunkHeaderID,
                            int                   startIndex,
                            Endianness            endianness = Endianness::LittleEndian);

    //=============================================================
    uint32_t getAiffSampleRate(std::vector<uint8_t>& fileData, int sampleRateStartIndex);
    bool     tenByteMatch(std::vector<uint8_t>& v1, int startIndex1, std::vector<uint8_t>& v2, int startIndex2);
    void     addSampleRateToAiffData(std::vector<uint8_t>& fileData, uint32_t sampleRate);

    //=============================================================
    void addStringToFileData(std::vector<uint8_t>& fileData, std::string s);
    void addInt32ToFileData(std::vector<uint8_t>& fileData, int32_t i, Endianness endianness = Endianness::LittleEndian);
    void addInt16ToFileData(std::vector<uint8_t>& fileData, int16_t i, Endianness endianness = Endianness::LittleEndian);

    //=============================================================
    bool writeDataToFile(std::vector<uint8_t>& fileData, std::string filePath);

    //=============================================================
    void reportError(std::string errorMessage);

    //=============================================================
    AudioFileFormat audioFileFormat;
    uint32_t        sampleRate;
    int             bitDepth;
    bool            logErrorsToConsole{true};
};

//=============================================================
template <typename T> struct AudioSampleConverter {
    //=============================================================
    /** Convert a signed 8-bit integer to an audio sample */
    static T signedByteToSample(int8_t sample);

    /** Convert an audio sample to an signed 8-bit representation */
    static int8_t sampleToSignedByte(T sample);

    //=============================================================
    /** Convert an unsigned 8-bit integer to an audio sample */
    static T unsignedByteToSample(uint8_t sample);

    /** Convert an audio sample to an unsigned 8-bit representation */
    static uint8_t sampleToUnsignedByte(T sample);

    //=============================================================
    /** Convert a 16-bit integer to an audio sample */
    static T sixteenBitIntToSample(int16_t sample);

    /** Convert a an audio sample to a 16-bit integer */
    static int16_t sampleToSixteenBitInt(T sample);

    //=============================================================
    /** Convert a 24-bit value (int a 32-bit int) to an audio sample */
    static T twentyFourBitIntToSample(int32_t sample);

    /** Convert a an audio sample to a 24-bit value (in a 32-bit integer) */
    static int32_t sampleToTwentyFourBitInt(T sample);

    //=============================================================
    /** Convert a 32-bit signed integer to an audio sample */
    static T thirtyTwoBitIntToSample(int32_t sample);

    /** Convert a an audio sample to a 32-bit signed integer */
    static int32_t sampleToThirtyTwoBitInt(T sample);

    //=============================================================
    /** Helper clamp function to enforce ranges */
    static T clamp(T v1, T minValue, T maxValue);
};

//=============================================================
// Pre-defined 10-byte representations of common sample rates
static std::unordered_map<uint32_t, std::vector<uint8_t>> aiffSampleRateTable = {
    {   8000,   {64, 11, 250, 0, 0, 0, 0, 0, 0, 0}},
    {  11025,  {64, 12, 172, 68, 0, 0, 0, 0, 0, 0}},
    {  16000,   {64, 12, 250, 0, 0, 0, 0, 0, 0, 0}},
    {  22050,  {64, 13, 172, 68, 0, 0, 0, 0, 0, 0}},
    {  32000,   {64, 13, 250, 0, 0, 0, 0, 0, 0, 0}},
    {  37800, {64, 14, 147, 168, 0, 0, 0, 0, 0, 0}},
    {  44056,  {64, 14, 172, 24, 0, 0, 0, 0, 0, 0}},
    {  44100,  {64, 14, 172, 68, 0, 0, 0, 0, 0, 0}},
    {  47250, {64, 14, 184, 146, 0, 0, 0, 0, 0, 0}},
    {  48000, {64, 14, 187, 128, 0, 0, 0, 0, 0, 0}},
    {  50000,  {64, 14, 195, 80, 0, 0, 0, 0, 0, 0}},
    {  50400, {64, 14, 196, 224, 0, 0, 0, 0, 0, 0}},
    {  88200,  {64, 15, 172, 68, 0, 0, 0, 0, 0, 0}},
    {  96000, {64, 15, 187, 128, 0, 0, 0, 0, 0, 0}},
    { 176400,  {64, 16, 172, 68, 0, 0, 0, 0, 0, 0}},
    { 192000, {64, 16, 187, 128, 0, 0, 0, 0, 0, 0}},
    { 352800,  {64, 17, 172, 68, 0, 0, 0, 0, 0, 0}},
    {2822400,  {64, 20, 172, 68, 0, 0, 0, 0, 0, 0}},
    {5644800,  {64, 21, 172, 68, 0, 0, 0, 0, 0, 0}}
};

//=============================================================
enum WavAudioFormat { PCM = 0x0001, IEEEFloat = 0x0003, ALaw = 0x0006, MULaw = 0x0007, Extensible = 0xFFFE };

//=============================================================
enum AIFFAudioFormat { Uncompressed, Compressed, Error };

#if defined(_MSC_VER)
__pragma(warning(pop))
#elif defined(__GNUC__)
_Pragma("GCC diagnostic pop")
#endif

#endif  // DUMMYENGINE_AUDIOFILE_H
