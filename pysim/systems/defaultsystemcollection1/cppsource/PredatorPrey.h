#pragma once
#include "CppSystem.hpp"

class PredatorPrey :
    public pysim::CppSystem
{
public:
    PredatorPrey(void);

    static std::string getDocs();

    //First calculation in a simulation, only done once
    void preSim(){y=y0; x=x0;};

    //This function is called for each evaluation
    void doStep(double time);

    //After each set of evaluations when a new state value 
    //has been calculated this section is called
    void postStep(){};

    //Before finishing the simulation this function is called
    void finally(){};

protected:

    //Parameters
    double alfa, beta, gamma, delta, x0, y0;
    
    //States
    double y,dy;
    double x,dx;
};