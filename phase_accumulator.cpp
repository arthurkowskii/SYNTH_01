#include <iostream>
#include <fstream>
#include <cmath>




int main(){
    const float frequency = 2;
    const int samplerate = 10;
    const float phaseIncrement = frequency / samplerate;
    const float Pi = 3.14159265358979323846f;
    float sample;
    float phase = 0.0f;

    for (int i = 0; i < samplerate; ++i){
        sample = sin(2 * Pi * phase);
        std::cout << "phase is " << phase << " & index is " << i << "sample is " << sample << " \n";
        phase += phaseIncrement;
        if (phase >= 1){
         phase -= 1.0f;
        }
    }
    return 0;
}
