#pragma once
#include "CppSystem.hpp"

class SquareWave :
    public pysim::CppSystem
{
public:
    SquareWave(void);

    static std::string getDocs();

    void doStep(double time);

protected:

    //Parameters
    double amplitude;
    double freq;
    double phaseRad;

    //Outputs
    double signal;

};