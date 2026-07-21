#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <cmath>
#include <stdexcept>

void create16bit_char(unsigned char* buffer, std::string str){
    buffer[0] = str[0];
    buffer[1] = str[1];
}

void create32bit_char (unsigned char* buffer, std::string str){
    buffer[0] = str[0];
    buffer[1] = str[1];
    buffer[2] = str[2];
    buffer[3] = str[3];
}

void create16bit_Int(unsigned char* buffer, uint16_t value){
    buffer[0] = static_cast<unsigned char>(value & 0xFF);
    buffer[1] = static_cast<unsigned char>((value >> 8) & 0xFF);
}

void create32bit_Int(unsigned char* buffer, uint32_t value){
    buffer[0] = static_cast<unsigned char>(value & 0xFF);
    buffer[1] = static_cast<unsigned char>((value >> 8) & 0xFF);
    buffer[2] = static_cast<unsigned char>((value >> 16) & 0xFF);
    buffer[3] = static_cast<unsigned char>((value >> 24) & 0xFF);
}

void writingBits(unsigned char* buffer, uint32_t pcmValue, unsigned bits){
    const unsigned bytes = bits / 8;
    for (unsigned i = 0; i < bytes; ++i){
        buffer[i] = static_cast<unsigned char>((pcmValue >> (i * 8)) & 0xFF);
    }
}

int32_t floatToPCMConverter(float sinValue, unsigned bits){
    if (bits == 8){
        int value = static_cast<int>(std::round(sinValue * 127.0f)) + 128;
        if (value < 0)  value = 0;
        if (value > 255) value = 255;
        return value;
    }
    const int64_t maxValue = (int64_t{1} << (bits - 1)) - 1;
    return static_cast<int32_t>(std::round(sinValue * maxValue));
}

void writingSin(uint32_t samplerate, std::vector<unsigned char>& audioData, float frequency, unsigned bits){
    if (bits != 8 && bits != 16 && bits != 32){
        throw std::invalid_argument("Only 8-bit, 16-bit and 32-bit audio are supported");
    }

    const size_t bytesPerSample = bits / 8;
    const float phaseIncrement = frequency / static_cast<float>(samplerate);
    float phase = 0.0f;
    constexpr float PI = 3.14159265358979323846f;

    for (size_t i = 0; i + bytesPerSample <= audioData.size(); i += bytesPerSample){
        int32_t pcmValue = floatToPCMConverter(std::sin(2.0f * PI * phase), bits);
        writingBits(&audioData[i], static_cast<uint32_t>(pcmValue), bits);
        phase += phaseIncrement;
        if (phase >= 1.0f){
            phase -= 1.0f;
        }
    }
}

void writingSquare(uint32_t sampleRate, std::vector<unsigned char>& audioData, float frequency, unsigned bits){
    const size_t bytesPerSample = bits / 8;
    const float phaseIncrement = frequency / sampleRate;
    float phase = 0;
    float squareValue = -1;
    unsigned int i = 0;
    float PCMValue = floatToPCMConverter(squareValue, bits);

    for (i=0; i<audioData.size(); i+=bytesPerSample){
        if (phase >=0.5){
            squareValue = -1;
        }else{
            squareValue = 1;
        }
        PCMValue = floatToPCMConverter(squareValue, bits);
        writingBits(&audioData[i], static_cast<uint32_t>(PCMValue), bits);
        phase += phaseIncrement;
        if (phase >= 1.0f){
            phase -= 1.0f;
        }
    }
}

void writingTriangle(uint32_t sampleRate, std::vector<unsigned char>& audioData, float frequency, unsigned bits){
    const size_t bytesPerSample = bits / 8;
    const float phaseIncrement = frequency / sampleRate;
    float phase = 0;
    float triangleValue = -1;
    unsigned int i = 0;
    float PCMValue = floatToPCMConverter(triangleValue, bits);

    for (i=0; i<audioData.size();i+=bytesPerSample)
    {
        if (phase <0.5){
            triangleValue = -1.0f + 4.0f * phase;
            PCMValue = floatToPCMConverter(triangleValue, bits);
            writingBits(&audioData[i], static_cast<uint32_t>(PCMValue), bits);

        }else {
            triangleValue = 3.0f - 4.0f * phase;
            PCMValue = floatToPCMConverter(triangleValue, bits);
            writingBits(&audioData[i], static_cast<uint32_t>(PCMValue), bits);
        }
        phase += phaseIncrement;
        if (phase >= 1.0f){
            phase -= 1.0f;
        }
    }
}

void wavMaker(int channels, int bits, int sampleRate, const std::vector<unsigned char>& audioData){
    std::vector<unsigned char> buffer(44);
    uint32_t dataSize = audioData.size();
    create32bit_char(&buffer[0], "RIFF"); // RIFF HEADER
    create32bit_Int(&buffer[4], dataSize + 36); // RIFF chunk size = file size - 8
    create32bit_char(&buffer[8], "WAVE"); // FORMAT
    create32bit_char(&buffer[12], "fmt "); // Subchunk1 ID
    create32bit_Int(&buffer[16], 16); // Subchunk 1 Size
    create16bit_Int(&buffer[20], 1); // Audio format : PCM lossless
    create16bit_Int(&buffer[22], channels); // CHANNELS
    create32bit_Int(&buffer[24], sampleRate); // SAMPLE RATE
    create32bit_Int(&buffer[28], sampleRate * channels * (bits / 8)); // BYTE RATE
    create16bit_Int(&buffer[32], channels * bits / 8); // BLOCK ALIGNMENT
    create16bit_Int(&buffer[34], bits); // BITS PER SAMPLE
    create32bit_char(&buffer[36], "data"); // Subchunk 2 ID (DATA)
    create32bit_Int(&buffer[40], dataSize); // Subchunk 2 Size (DATA SIZE)

    std::cout << "Successfully created the header \n";

    std::ofstream outputWav("audioData.wav", std::ios::binary);
    std::cout << "Empty wav file created \n";
    outputWav.write(reinterpret_cast<const char*>(&buffer[0]), buffer.size());
    std::cout << "Header written on the wav \n";
    outputWav.write(reinterpret_cast<const char*>(&audioData[0]), dataSize);
    std::cout << "audioData written on the wav \n";
    outputWav.close();
}

int main(){
    int frequency = 440;
    uint32_t samplerate = 48000;
    uint16_t channels = 1;
    uint16_t bits = 16;
    uint32_t durationSeconds = 2;
    uint32_t frames = durationSeconds * samplerate;
    uint32_t dataSize = frames * channels * (bits / 8);

    std::vector<unsigned char> audioData(dataSize);
    writingTriangle(samplerate, audioData, frequency, bits);
    wavMaker(channels, bits, samplerate, audioData);

    return 0;
}
