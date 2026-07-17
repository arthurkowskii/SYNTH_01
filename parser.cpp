#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>

uint16_t bitReader16(char* buffer){
    return ((unsigned char)buffer[0]) + ((unsigned char)buffer[1] << 8);
}

uint32_t bitReader32(char* buffer){
    return ((unsigned char)buffer[0]) + ((unsigned char)buffer[1] << 8) + ((unsigned char)buffer[2] << 16) + ((unsigned char)buffer[3] << 24);
}

bool wavAnalyser(std::string fileName){
    uint32_t samplerate;
    int numChannels;
    uint16_t bitsPerSample;
    std::string format;
    std::vector<char> audioBuffer(44);
    std::ifstream audioFile(fileName, std::ios::binary);

    if (audioFile.read(audioBuffer.data(), 44)) {
        samplerate = bitReader32(&audioBuffer[24]);
        bitsPerSample = bitReader16(&audioBuffer[34]);
        numChannels = bitReader16(&audioBuffer[22]);
        std::cout << "bits per sample is " << bitsPerSample << "\n";
        std::cout << "samplerate is " << samplerate << "\n";
        std::cout << "num channels is " << numChannels << "\n";
        return true;
    }

    return false;
}

int main(){
    std::string fileName = "silence.wav";

    if (wavAnalyser(fileName)){
        std::cout << "wav analyser started successfully \n";
    }else {
        std::cout << "error trying to start wav analyser";
    }
    return 0;
}
