#pragma once
#include "CppSystem.hpp"

class DiscretePID :
    public pysim::CppSystem
{
public:
    DiscretePID(void);
    static std::string getDocs();
    virtual void preSim();
    void doStep(double time);

protected:

    //Parameters
    double stepsize;
    double p,plim,i,d;
    double antiwindup;
    double insig, refsig,dsig;

    //State
    double iPart,dIPart;

    //Outputs
    double outsig;;

};