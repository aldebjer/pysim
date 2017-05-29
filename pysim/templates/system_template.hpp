#pragma once
#include "CppSystem.hpp"

class {{systemname}} :
    public pysim::CppSystem
{
public:
    {{systemname}}(void);

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

    //States and Ders
    {% for state in states %}{{state.type_name}} {{state.state_name}}, {{state.der_name}};
    {% endfor %}
    //Inputs
    {% for input in inputs %}{{input.type_name}} {{input.name}};
    {% endfor %}
    //Outputs
    {% for output in outputs %}{{output.type_name}} {{output.name}};
    {% endfor %}
};
