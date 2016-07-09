#pragma once
#include "CppSystem.hpp"

class LogisticMap :
    public pysim::CppSystem
{
public:
    LogisticMap(void);
    virtual void preSim();
    void doStep(double time);

protected:

    //Parameters
    double stepsize;
    double r;

    //States
    double x, xnext;


};