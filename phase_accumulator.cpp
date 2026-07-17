#include <iostream>
#include <fstream>

int main(){
    const float frequency = 2;
    const int samplerate = 10;
    const float phaseIncrement = frequency / samplerate;
    float phase = 0.0f;

    for (int i = 0; i < samplerate; ++i){
        std::cout << "phase is " << phase << " & index is " << i << " \n";
        phase += phaseIncrement;
        if (phase >= 1){
         phase -= 1.0f;
        }
    }
    return 0;
}
