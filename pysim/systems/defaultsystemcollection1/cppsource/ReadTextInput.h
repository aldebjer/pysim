#pragma once
#include "CppSystem.hpp"

class ReadTextInput :
    public pysim::CppSystem
{
public:
	ReadTextInput(void);

    static std::string getDocs();

    //First calculation in a simulation, only done once
    void preSim();

    //This function is called for each evaluation
    void doStep(double time);

    //After each set of evaluations when a new state value 
    //has been calculated this section is called
    void postStep(){};

    //Before finishing the simulation this function is called
    void finally(){};

    static const size_t NUMBER_OF_COLUMNS = 20;

protected:

    //Inputs
    std::string filename;
    
    //output;
    double columns[NUMBER_OF_COLUMNS];

    //Map were keys are the timestamp and each element contains the stored
    //Values for that time.
    std::map<double, std::vector<double> > valuemap;
};