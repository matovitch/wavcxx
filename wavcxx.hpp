#ifndef __WAVCXX_H__
#define __WAVCXX_H__

#include <stdexcept>
#include <iostream>
#include <cstddef>
#include <fstream>
#include <limits>
#include <vector>
#include <array>
#include <ios>

namespace wavcxx
{

typedef uint8_t Byte;

struct Endianness
{
    Endianness()
    {
        uint16_t i = 1;
        _endianness = (reinterpret_cast<Byte*>(&i)[0] == 1);
    }

    enum : bool
    {
        BIG    = false,
        LITTLE = true
    };

    bool operator== (bool endianness) const 
    {
        return _endianness == endianness;
    }

    bool _endianness;
};

typedef std::array<Byte, 4> HeaderMagic;

static const HeaderMagic MAGIC_RIFF = { 0x52, 0x49, 0x46, 0x46};
static const HeaderMagic MAGIC_WAVE = { 0x57, 0x41, 0x56, 0x45};
static const HeaderMagic MAGIC_FMT_ = { 0x66, 0x6D, 0x74, 0x20};
static const HeaderMagic MAGIC_BKSZ = { 0x10, 0x00, 0x00, 0x00};
static const HeaderMagic MAGIC_DATA = { 0x64, 0x61, 0x74, 0x61};

typedef uint32_t FileSize;

static const FileSize OFFSET_SIZE =  8;
static const FileSize HEADER_SIZE = 44;

static const Byte FORMAT_PCM = 1;

static constexpr std::size_t MAX_CHANNELS = 6;

class File
{

public:

    struct Config
    {
        Config() :
            _frequency(44100),
            _nChannels(    1),
            _bitDepth (   16)
        {}

        Config(uint32_t frequency,
               Byte     nChannels,
               Byte     bitDepth) :
            _frequency(frequency),
            _nChannels(nChannels),
            _bitDepth (bitDepth )
        {}

        uint32_t _frequency;
        Byte     _nChannels;
        Byte     _bitDepth;
    };

    File(const std::string& path,
         const Config& config = Config()) :
        _byteDepth(config._bitDepth / std::numeric_limits<Byte>::digits),
        _path(path)
    {
        push(MAGIC_RIFF);
        padd(sizeof(FileSize));
        push(MAGIC_WAVE);
        push(MAGIC_FMT_);
        push(MAGIC_BKSZ);
        push(FORMAT_PCM);
        padd(1);
        push(config._nChannels);
        padd(1);
        push(config._frequency);

        const Byte bytesPerBlock = config._nChannels * _byteDepth;

        push(bytesPerBlock * config._frequency);
        push(bytesPerBlock);
        padd(1);
        push(config._bitDepth);
        padd(1);
        push(MAGIC_DATA);
        padd(sizeof(FileSize));
    }

    ~File()
    {
        flushChannels();

        headerRewrite(OFFSET_SIZE);
        headerRewrite(HEADER_SIZE);

        std::ofstream file(_path, std::ios_base::out | std::ios::binary);

        if (file.is_open())
        {
            file.write(reinterpret_cast<char*>(_data.data()), _data.size());
        }
        else
        {
            throw std::runtime_error("Failed to write WAVE file to " + _path + ".");
        }
    }

    template <typename T>
    void pushChannel(std::size_t channelId, T toPush, std::size_t padd = 0)
    {
        std::vector<Byte>& channel = _channels[channelId];

        channel.resize(channel.size() + sizeof(T));

        endianAwareWrite(reinterpret_cast<const Byte*>(&toPush),
                         &(channel.back()) - sizeof(T) + 1,
                         sizeof(T));
    }

private:

    template <class T>
    void push(const T& toPush)
    {
        padd(sizeof(T));

        endianAwareWrite(reinterpret_cast<const Byte*>(&toPush),
                         &(_data.back()) - sizeof(T) + 1,
                         sizeof(T));
    }

    void push(Byte byte)
    {
        _data.push_back(byte);
    }

    void padd(const std::size_t size)
    {
        _data.resize(_data.size() + size);
    }

    void flushChannels()
    {
        for (std::size_t i = 0; i < _channels[0].size(); i++)
        {
            for (const std::vector<Byte>& channel : _channels)
            {
                for (std::size_t j = 0; j < _byteDepth; j++)
                {
                    const std::size_t index = i * _byteDepth + j;

                    if (index < channel.size())
                    {
                        _data.push_back(channel[index]);
                    }
                }
                
            }
        }  
    }

    void headerRewrite(const FileSize& offset)
    {
        FileSize fileSize = _data.size() - offset;

        endianAwareWrite(reinterpret_cast<const Byte*>(&fileSize),
                         _data.data() + offset - sizeof(FileSize), 
                         sizeof(FileSize));
    }

    void endianAwareWrite(const Byte* source, Byte* destination, std::size_t size)
    {
        if (Endianness() == Endianness::LITTLE)
        {
            for (std::size_t i = 0; i < size; i++)
            {
                destination[i] = source[i];
            }
        }

        if (Endianness() == Endianness::BIG)
        {
            for (std::size_t i = size - 1; i >= 0; i--)
            {
                destination[i] = source[i];
            }
        }
    }

    unsigned          _byteDepth;
    std::vector<Byte> _data;
    std::string       _path;
    std::array<std::vector<Byte>, MAX_CHANNELS> _channels;

};

} // end wavlib namespace

#endif // end __WAVCXX_H__