#pragma once
#include <vector>

#include "SimulatableSystem.hpp"

class  CythonSystemImpl :
    public SimulatableSystem
{
public:
    virtual ~CythonSystemImpl(void){};

    void preSim(){};
    void doStep(double time);
    void doStoreStep(double time);
    void copyoutputs();
    void copystateoutputs();

    std::vector<double*> getStatePointers();
    std::vector<double*> getDerPointers();

    double getNextUpdateTime();
    bool getDiscrete();

    bool do_comparison();

    std::vector<double*> states;
    std::vector<double*> ders;

    //void addStepCallback()


};

