#include "wavcxx.hpp"

#include <cstddef>
#include <cmath>

typedef uint16_t            Sample ; // As wavcxx::DEFAULT_BIT_DEPTH == 16.
typedef std::vector<Sample> Samples;

static const float VOLUME    =  8000.0; // Max. 2^(wavcxx::DEFAULT_BIT_DEPTH) - 1;
static const float DURATION  =     5.0; // in seconds

void buildSamples(Samples& samples, float tone)
{
    const float FREQUENCY =  wavcxx::DEFAULT_SAMPLE_FREQUENCY;

    for (std::size_t i = 0; i < DURATION * FREQUENCY; i++)
    {
        samples.push_back(VOLUME * sin(2 * M_PI * tone * i / FREQUENCY));
    }
}

void writeSamples(const Samples& samples,
                  wavcxx::File& wavFile,
                  std::size_t side)
{
    for (const Sample sample : samples)
    {
        wavFile.pushChannel(sample, side);
    }
}

namespace Side
{
    enum
    {
        LEFT,  // 0 for the left channel
        RIGHT  // 1 for the right one
    };
}

int main()
{
    wavcxx::File::Config config = { wavcxx::DEFAULT_SAMPLE_FREQUENCY, // sampling frequency (44100)
                                    2,                                // number of channels (1 by default)
                                    wavcxx::DEFAULT_BIT_DEPTH };      // bit depth (16)

    wavcxx::File wavFile("./sound/stereo.wav", config);

    Samples lSamples, rSamples;

    buildSamples(lSamples, 440.000); // A4 key 
    buildSamples(rSamples, 659.255); // E5 key

    writeSamples(lSamples, wavFile, Side::LEFT);
    writeSamples(rSamples, wavFile, Side::RIGHT);

    return 0;
}