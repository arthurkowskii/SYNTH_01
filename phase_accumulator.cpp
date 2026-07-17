#include <iostream>
#include <fstream>


int main(){
    float frequency = 2;
    float samplerate = 10;
    float phase = 0.0f;

    for (int i = 0; i < 10; ++i){
        std::cout << "phase is " << phase << " & index is " << i << " \n";
        phase += frequency / samplerate;

        if (phase >= 1){
         phase -= 1.0f;
        }

    }

    return 0;
}
