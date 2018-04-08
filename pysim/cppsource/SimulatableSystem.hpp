#pragma once
#include <vector>

namespace pysim {

class SimulatableSystemInterface
{
public:
    virtual ~SimulatableSystemInterface(void){};

    virtual void preSim(){};
	virtual void preStep() {};
    virtual void doStep(double time)=0;
    virtual void postStep(){};
    virtual void doStoreStep(double time) = 0;
    virtual void copyoutputs() = 0;
    virtual void copystateoutputs() = 0;

    virtual std::vector<double*> getStatePointers() = 0;
    virtual std::vector<double*> getDerPointers() = 0;

    virtual double getNextUpdateTime() = 0;
    virtual bool getDiscrete() = 0;

    virtual bool do_comparison() = 0;

};

}
