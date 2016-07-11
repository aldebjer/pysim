#include "SquareWave.h"

#include <cmath>
#include "factory.hpp"

std::string SquareWave::getDocs(){
    return std::string("TODO: Add this");
}


SquareWave::SquareWave(void)
{
    INPUT(amplitude, "Amplitude of the out-signal")
    INPUT(freq, "Frequency of the out-signal")
    INPUT(phaseRad, "Phase shift of the out-signal, in radians")

    OUTPUT(signal, "Square wave output signal")

    amplitude = 1;
    freq = 1;
    phaseRad = 0;
    signal = 0;

}

void SquareWave::doStep(double time){
    double sinsignal = sin((time)*freq + phaseRad);
    if (sinsignal < 0){
        signal = amplitude;
    }
    else{
        signal = 0;
    }
}

REGISTER_SYSTEM(SquareWave);