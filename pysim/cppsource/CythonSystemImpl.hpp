#pragma once
#include <vector>

#include "CommonSystemImpl.hpp"

namespace pysim{

class  CythonSystemImpl :
    public CommonSystemImpl
{
public:
    CythonSystemImpl();
    ~CythonSystemImpl(){};

    void preSim(){};
    void doStep(double time);

    double getNextUpdateTime();

    bool do_comparison();



    void* sysp;

    //Python Interface
    void add_input_vector(std::string name, size_t length);
    void add_output_vector(std::string name, size_t length);
    void add_state_vector(std::string statename, std::string dername, size_t length);

};

}
