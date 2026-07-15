#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>

bool wavAnalyser(std::string fileName){
    int samplerate;
    int numChannels;
    int bitsPerSample;
    std::string format;
    std::vector<char> audioBuffer(44);
    std::ifstream audioFile(fileName, std::ios::binary);

    if (audioFile.read(audioBuffer.data(), 44)) {
        samplerate = audioBuffer[24] + (audioBuffer[25] << 8) + (audioBuffer[26] << 16) + (audioBuffer[27] << 24);
        bitsPerSample = audioBuffer[34] + (audioBuffer[35] << 8);
        numChannels = audioBuffer[28];
        format = std::string(audioBuffer.begin() + 8, audioBuffer.begin() + 12);
        std::cout << "sample rate is : " << samplerate << " and bits per sample is : " << bitsPerSample << " and num channels is : " << numChannels << " and format is : " << format << "\n";
        return true;
    }

    return false;
}

int main(std::string fileName){
    std::string fileName = "audio.wav";

    if (wavAnalyser(fileName)){
        std::cout << "wav analyser started successfully \n";
    }else {
        std::cout << "error trying to start wav analyser";
    }
    return 0;
}
