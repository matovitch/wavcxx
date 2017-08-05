#include "wavcxx.hpp"

#include <cstddef>
#include <cmath>

typedef uint16_t            Sample ; // As wavcxx::DEFAULT_BIT_DEPTH == 16.
typedef std::vector<Sample> Samples;

static const float VOLUME    =  8000.0; // Max. 2^(wavcxx::DEFAULT_BIT_DEPTH) - 1;
static const float TONE      =   440.0; // in hertz
static const float DURATION  =     5.0; // in seconds

void buildSamples(Samples& samples)
{
    const float FREQUENCY = wavcxx::DEFAULT_SAMPLE_FREQUENCY;

    for (std::size_t i = 0; i < DURATION * FREQUENCY; i++)
    {
        samples.push_back(VOLUME * sin(2 * M_PI * TONE * i / FREQUENCY));
    }
}

void writeSamples(const Samples& samples, wavcxx::File& wavFile)
{
    for (const Sample sample : samples)
    {
        wavFile.pushChannel(sample);
    }
}

int main()
{
    wavcxx::File wavFile("./sound/sine_wave.wav");
    Samples samples;

    buildSamples(samples);
    writeSamples(samples, wavFile);

    return 0;
}