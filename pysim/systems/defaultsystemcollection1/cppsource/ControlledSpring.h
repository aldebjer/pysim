#pragma once
#include "MassSpringDamper.h"
#include "SquareWave.h"

class ControlledSpring :
    public pysim::CppSystem
{
public:
    ControlledSpring(void);

    static std::string getDocs();

    void preSim() {};
    void preStep() {};
    void doStep(double time);
    void postStep() {};

    void copyinputs();
    void copyoutputs();
    void copystateoutputs();


protected:

    //Inputs
    double amp;

    //Outputs
    double out;
    double signal;

    //Subsystems
    MassSpringDamper msd;
    SquareWave sqw;
};