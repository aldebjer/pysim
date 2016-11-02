#pragma once
#include "CppSystem.hpp"

class ParameterTestSystem :
    public pysim::CppSystem
{
public:
    ParameterTestSystem(void);

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
    std::vector<double> parameter_vector;
    std::vector<std::vector<double>> parameter_matrix;
    std::map<std::string, double> parameter_map;
    std::map<std::string, std::vector<double>> parameter_vectormap;

    double output_from_vector;
    double output_from_matrix;
    double output_from_map;
    double output_from_vectormap;


};