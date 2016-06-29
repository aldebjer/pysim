#pragma once
#include <vector>

#include "CommonSystemImpl.hpp"


class  CythonSystemImpl :
    public CommonSystemImpl
{
public:
    CythonSystemImpl();
    ~CythonSystemImpl(){};

    void preSim(){};
    void doStep(double time);
    void doStoreStep(double time);

    std::vector<double*> getStatePointers();
    std::vector<double*> getDerPointers();

    double getNextUpdateTime();
    bool getDiscrete();

    bool do_comparison();

    std::vector<double*> states;
    std::vector<double*> ders;

    void* sysp;

    //Python Interface
    void add_input_vector(std::string name, size_t length);
    void add_output_vector(std::string name, size_t length);

};

