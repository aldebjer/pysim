#include "LogisticMap.h"


#include "factory.hpp"

std::string LogisticMap::getDocs(){
    return std::string("TODO: Add this");
}

LogisticMap::LogisticMap(void):
    stepsize(1.0),
    r(2.0)
{
    INPUT(stepsize,"stepsize for the controller")
    INPUT(r,"coefficient")


    STATE(x, xnext,"Integrated part")

    setDiscrete();
    setNextUpdate(stepsize);
}

void LogisticMap::preSim(){
}

void LogisticMap::doStep(double time){
    xnext = r*x*(1.0 - x);

    setNextUpdate(time + stepsize);
}

REGISTER_SYSTEM(LogisticMap);
