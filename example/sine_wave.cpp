#include "wavcxx.hpp"

#include <cstddef>
#include <cmath>

void write(const std::vector<uint16_t>& samples,
           wavcxx::File& wavFile)
{
    for (auto sample : samples)
    {
        wavFile.pushChannel(0, sample);
    }
}

static const float FREQUENCY = 44100.0;
static const float VOLUME    =  8000.0;
static const float TONE      =   440.0;
static const float DURATION  =     5.0;

void buildSamples(std::vector<uint16_t>& samples)
{
    for (std::size_t i = 0; i < static_cast<std::size_t>(DURATION * FREQUENCY); i++)
    {
        samples.push_back(static_cast<uint16_t>(VOLUME * sin(2 * M_PI * TONE * i / FREQUENCY)));
    }
}

int main()
{
    wavcxx::File wavFile("./sound/sine_wave.wav");

    std::vector<uint16_t> samples;

    buildSamples(samples);

    write(samples, wavFile);

    return 0;
}