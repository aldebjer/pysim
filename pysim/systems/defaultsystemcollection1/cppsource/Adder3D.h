#pragma once
#include "CppSystem.hpp"

class Adder3D :
    public pysim::CppSystem
{
public:
    Adder3D(void);

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

    //Parameters
    pysim::vector input1, input2;

    //Variables
    pysim::vector output1;
};