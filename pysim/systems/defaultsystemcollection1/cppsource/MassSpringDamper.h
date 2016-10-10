#pragma once
#include "CppSystem.hpp"

class MassSpringDamper :
    public pysim::CppSystem
{
public:
    MassSpringDamper(void);

    static std::string getDocs();

    //First calculation in a simulation, only done once
    void preSim(){};

    //This function is called for each evaluation
    void doStep(double time);

    //After each set of evaluations when a new state value 
    //has been calculated this section is called
    void postStep(){};

    //Before finishing the simulation this function is called
    void finally(){};

protected:

    //Inputs
    double m,b,k,f;
    
    //States
    double x1,dx1,x2,dx2;

    //Outputs
    double acceleration;

};
