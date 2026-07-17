#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>


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

std::vector<unsigned char> wavMaker(int channels, int bits, int sampleRate, uint32_t dataSize, int durationSeconds){
    std::vector<unsigned char> buffer(44);
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

    std::vector<unsigned char> silence(dataSize);
    std::ofstream outputWav("silence.wav", std::ios::binary);
    outputWav.write(reinterpret_cast<const char*>(&buffer[0]), buffer.size());
    outputWav.write(reinterpret_cast<const char*>(&silence[0]), silence.size());
    outputWav.close();

    return buffer;
}

int main(){
    uint32_t samplerate = 44100;
    uint16_t channels = 1;
    uint16_t bits = 16;
    uint32_t durationSeconds = 1;
    uint32_t frames = durationSeconds * samplerate;
    uint32_t dataSize = frames * channels * (bits / 8);

    wavMaker(channels, bits, samplerate, dataSize, durationSeconds);

    return 0;
}
