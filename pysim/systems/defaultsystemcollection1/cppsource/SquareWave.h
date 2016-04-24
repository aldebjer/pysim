#pragma once
#include "CppSystem.hpp"

class SquareWave :
    public CppSystem
{
public:
    SquareWave(void);

    void doStep(double time);

protected:

    //Parameters
    double amplitude;
    double freq;
    double phaseRad;

    //Outputs
    double signal;

};