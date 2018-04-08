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
    void copystateoutputs();
    std::vector<double*> getStatePointers();
    std::vector<double*> getDerPointers();
	void postStep();
    void doStoreStep(double time);
    bool getDiscrete() { return false; };

    void preSim();
	void preStep();
    void doStep(double time);
    double getNextUpdateTime();
    bool do_comparison();

    //Python Interface
    void store(char* name);
    StoreHandler* getStoreHandlerP();

    void add_subsystem(CommonSystemImpl* subsystem, std::string name);
    void add_composite_subsystem(CompositeSystemImpl* subsystem, std::string name);

    void add_scalar_port_in(std::string name, double initial_value, std::string description);
    void add_vector_inport(std::string name, std::vector<double> initial_value, std::string description);
    void add_matrix_inport(std::string name, std::vector<std::vector<double>> initial_value, std::string description);

    void add_outport(std::string name, double initial_value, std::string description);
    void add_outport(std::string name, std::vector<double> initial_value, std::string description);
    void add_outport(std::string name, std::vector<std::vector<double>> initial_value, std::string description);

    void connect_port_in(std::string portname, CommonSystemImpl* subsystem, std::string subsystem_input);
    void connect_port_in_composite(std::string portname, CompositeSystemImpl* subsystem, std::string subsystem_input);
    void connect_port_out(std::string portname, CommonSystemImpl* subsystem, std::string subsystem_input);
    void connect_port_out_composite(std::string portname, CompositeSystemImpl* subsystem, std::string subsystem_input);

    Variable inputs;
    Variable outputs;

    ConnectionHandler connectionHandler;

protected:
    std::unique_ptr<CompositeSystemImplPrivate> d_ptr;

	void copyinternalinputs();
	void copyinternaloutputs();
};

}
