#pragma once
#include <vector>

#include "SimulatableSystem.hpp"
#include "Variable.hpp"

#include "ConnectionHandler.hpp"

namespace pysim{

struct CompositeSystemImplPrivate;
class CommonSystemImpl;
class StoreHandler;

class  CompositeSystemImpl :
    public SimulatableSystemInterface
{
public:
    CompositeSystemImpl();
    virtual ~CompositeSystemImpl();

    //////////////////////////////////////////////////////////////////////////
    //      Inherited from Simulatable System
    //////////////////////////////////////////////////////////////////////////
    void copyoutputs();
    void copystateoutputs() {};
    std::vector<double*> getStatePointers();
    std::vector<double*> getDerPointers();
    void doStoreStep(double time);
    bool getDiscrete() { return false; };

    void preSim();
    void doStep(double time);
    double getNextUpdateTime();
    bool do_comparison();

    //Python Interface
    void store(char* name);
    StoreHandler* getStoreHandlerP();

    void add_subsystem(CommonSystemImpl* subsystem, std::string name);
    void add_input_port(std::string name, std::string subsystemname, std::string subsystem_input, std::string description);
    void add_output_port(std::string name, std::string subsystemname, std::string subsystem_output, std::string description);

    Variable inputs;
    Variable outputs;

    ConnectionHandler connectionHandler;

protected:
    std::unique_ptr<CompositeSystemImplPrivate> d_ptr;
};

}
