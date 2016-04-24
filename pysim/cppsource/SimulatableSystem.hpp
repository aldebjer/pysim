#pragma once
#include <vector>

class System
{
public:
    virtual ~System(void){};

    virtual void preSim(){};
    virtual void doStep(double time)=0;
    virtual void doStoreStep(double time) = 0;
    virtual void copyoutputs() = 0;
    virtual void copystateoutputs() = 0;

    virtual std::vector<double*> getStatePointers() = 0;
    virtual std::vector<double*> getDerPointers() = 0;

    virtual double* getFirstState() = 0;
    virtual double* getNextState() = 0;

    virtual double* getFirstDer() = 0;
    virtual double* getNextDer() = 0;

    virtual double getNextUpdateTime() = 0;
    virtual bool getDiscrete() = 0;

    virtual bool do_comparison() = 0;

};

