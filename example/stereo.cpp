#include "wavcxx.hpp"

#include <cstddef>
#include <cmath>

namespace Side
{
    enum
    {
        LEFT,  // 0 for the left channel
        RIGHT  // 1 for the right one
    };
}

void write(const std::vector<uint16_t>& samples,
           wavcxx::File& wavFile,
           std::size_t side)
{
    for (auto sample : samples)
    {
        wavFile.pushChannel(side, sample);
    }
}

static const float FREQUENCY = 44100.0;
static const float VOLUME    =  8000.0;
static const float DURATION  =     5.0;

void buildSamples(std::vector<uint16_t>& samples, float tone)
{
    for (std::size_t i = 0; i < static_cast<std::size_t>(DURATION * FREQUENCY); i++)
    {
        samples.push_back(static_cast<uint16_t>(VOLUME * sin(2 * M_PI * tone * i / FREQUENCY)));
    }
}

int main()
{
    wavcxx::File::Config config = { 44100,      // sampling frequency
                                        2,      // number of channels
                                       16 };    // bit depth

    wavcxx::File wavFile("./sound/stereo.wav", config);

    std::vector<uint16_t> lSamples;
    std::vector<uint16_t> rSamples;

    buildSamples(lSamples, 440.000); // A4 key
    buildSamples(rSamples, 659.255); // E5 key

    write(lSamples, wavFile, Side::LEFT);
    write(rSamples, wavFile, Side::RIGHT);

    return 0;
}