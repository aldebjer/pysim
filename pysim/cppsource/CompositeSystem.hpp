#pragma once
#include <vector>

#include "SimulatableSystem.hpp"
#include "Variable.hpp"

namespace pysim{

struct CompositeSystemPrivate;
class CommonSystemImpl;

class  CompositeSystem :
    public SimulatableSystem
{
public:
    CompositeSystem();
    virtual ~CompositeSystem();

    //////////////////////////////////////////////////////////////////////////
    //      Inherited from Simulatable System
    //////////////////////////////////////////////////////////////////////////
    void copyoutputs() {};
    void copystateoutputs() {};
    std::vector<double*> getStatePointers();
    std::vector<double*> getDerPointers();
    void doStoreStep(double time) {};
    bool getDiscrete() { return false; };


    void preSim();
    void doStep(double time);
    double getNextUpdateTime();
    bool do_comparison();

    //Python Interface
    void add_subsystem(CommonSystemImpl* subsystem, std::string name);
    void add_input_port(std::string name, size_t length);
    void add_output_port(std::string name, size_t length);

    Variable inputs;
    Variable outputs;

protected:
    std::unique_ptr<CompositeSystemPrivate> d_ptr;
};

}
