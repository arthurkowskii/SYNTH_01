#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <cmath>

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

int16_t floatToPCMConverter(float sinValue){
    return static_cast<int16_t>(std::round(sinValue * 32767.0f));
}

void writingSin(uint32_t samplerate, std::vector<unsigned char>& silence, float frequency){
    int i = 0;
    float maxValue = 0;
    float minValue = 0;
    int16_t PCMValue;
    float phaseIncrement = frequency / static_cast<float>(samplerate);
    float phase = 0;
    const float PI = 3.14159265358979323846;

    for (i = 0; i<silence.size(); i+=2){
        PCMValue = floatToPCMConverter(sin(2*PI*phase));
        create16bit_Int(&silence[i], PCMValue);
        phase += phaseIncrement;
        if (phase >= 1.0f){
            phase -= 1.0f;
        }
        if (PCMValue > maxValue){
            maxValue = PCMValue;
        }else if (PCMValue < minValue){
            minValue = PCMValue;
        }
    }
}

void wavMaker(int channels, int bits, int sampleRate, const std::vector<unsigned char>& silence){
    std::vector<unsigned char> buffer(44);
    uint32_t dataSize = silence.size();
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

    std::ofstream outputWav("silence.wav", std::ios::binary);
    std::cout << "Empty wav file created \n";
    outputWav.write(reinterpret_cast<const char*>(&buffer[0]), buffer.size());
    std::cout << "Header written on the wav \n";
    outputWav.write(reinterpret_cast<const char*>(&silence[0]), dataSize);
    std::cout << "Silence written on the wav \n";
    outputWav.close();
}

int main(){
    int frequency = 1200;
    uint32_t samplerate = 44100;
    uint16_t channels = 1;
    uint16_t bits = 16;
    uint32_t durationSeconds = 1;
    uint32_t frames = durationSeconds * samplerate;
    uint32_t dataSize = frames * channels * (bits / 8);

    std::vector<unsigned char> silence(dataSize);
    writingSin(samplerate, silence, frequency);
    wavMaker(channels, bits, samplerate, silence);

    return 0;
}
