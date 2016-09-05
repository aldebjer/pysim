#include "CompositeSystemImpl_p.hpp"

#include <map>

#include "CommonSystemImpl_p.hpp"

#include "Variable_p.hpp"

#include "PysimTypes.hpp"

#include <iostream>
#include <string>

#include "StoreHandler.hpp"

using std::string;

namespace pysim{


CompositeSystemImpl::CompositeSystemImpl():
    d_ptr(new CompositeSystemImplPrivate()),
    connectionHandler(&outputs)
{
}
CompositeSystemImpl::~CompositeSystemImpl()
{
}

//////////////////////////////////////////////////////////////////////////
//      Inherited from Simulatable System
//////////////////////////////////////////////////////////////////////////

std::vector<double*> CompositeSystemImpl::getStatePointers() {
    std::vector<double*> states;
    for (CommonSystemImpl* s : d_ptr->subsystems_common) {
        std::vector<double*> subs_states = s->getStatePointers();
        std::copy(subs_states.begin(), subs_states.end(), std::back_inserter(states));
    }
    return states;
};

std::vector<double*> CompositeSystemImpl::getDerPointers() {
    std::vector<double*> ders;
    for (CommonSystemImpl* s : d_ptr->subsystems_common) {
        std::vector<double*> subs_ders = s->getDerPointers();
        std::copy(subs_ders.begin(), subs_ders.end(), std::back_inserter(ders));
    }
    return ders;
};


void CompositeSystemImpl::preSim()
{
    for (CommonSystemImpl* s : d_ptr->subsystems_common) {
        s->preSim();
    }
}

void CompositeSystemImpl::doStep(double time)
{
    for (CommonSystemImpl* s : d_ptr->subsystems_common) {
        s->doStep(time);
    }
}

void CompositeSystemImpl::doStoreStep(double time) {
    d_ptr->storeHandler.doStoreStep(time);

    for (CommonSystemImpl* s : d_ptr->subsystems_common) {
        s->doStoreStep(time);
    }

}

double CompositeSystemImpl::getNextUpdateTime() 
{
    return 0;
}

bool CompositeSystemImpl::do_comparison()
{
    bool comparison_trigged = false;
    for (CommonSystemImpl* s : d_ptr->subsystems_common) {
        comparison_trigged = comparison_trigged || s->do_comparison();
    }
    return comparison_trigged;
}

////////////////////////////////////
//
//       Connections
//
////////////////////////////////////

void CompositeSystemImpl::copyoutputs() {

    for (CommonSystemImpl* s : d_ptr->subsystems_common) {
        s->copyoutputs();
    }
    connectionHandler.copyoutputs();
}

/////////////////////////////////////////////////////////////////////
//
//         Python Interface
//
/////////////////////////////////////////////////////////////////////


//Put the state, der, input or output named "name" in the vector of pointers 
//to be stored. If none with "name" is found the function raises an invalid_argument
//exception.
void  CompositeSystemImpl::store(char* name) {
    if (inputs.d_ptr->scalars.count(name) == 1) {
        d_ptr->storeHandler.store_scalar(name, inputs.d_ptr->scalars[name]);
    } else if (outputs.d_ptr->scalars.count(name) == 1) {
        d_ptr->storeHandler.store_scalar(name, outputs.d_ptr->scalars[name]);
    } else if (inputs.d_ptr->vectors.count(name) == 1) {
        d_ptr->storeHandler.store_vector(name, inputs.d_ptr->vectors[name]);
    } else if (outputs.d_ptr->vectors.count(name) == 1) {
        d_ptr->storeHandler.store_vector(name, outputs.d_ptr->vectors[name]);
    } else {
        char errmsg[50];
        snprintf(errmsg, sizeof(errmsg), "Could not store: %s, no such variable", name);
        throw std::invalid_argument(errmsg);
    }
}

StoreHandler* CompositeSystemImpl::getStoreHandlerP() {
    return &(d_ptr->storeHandler);
}

void CompositeSystemImpl::add_subsystem(CommonSystemImpl* subsystem, string name)
{
    if (subsystem->getDiscrete()) {
        throw std::invalid_argument("Discrete systems not supported as subsystems");
    }
    d_ptr->subsystems_common_map[name] = subsystem;
    d_ptr->subsystems_common.push_back(subsystem);
}

void CompositeSystemImpl::add_input_port(string name, string subsystemname, string subsystem_input, string description)
{
    double* ss_input_p = d_ptr->subsystems_common_map[subsystemname]->inputs.d_ptr->scalars[subsystem_input];

    inputs.d_ptr->scalars[name] = ss_input_p;
    inputs.d_ptr->descriptions[name] = description;
}

void CompositeSystemImpl::add_output_port(string name, string subsystemname, string subsystem_output, string description)
{
    std::map<string, double*>* scalar_output_map = &(d_ptr->subsystems_common_map[subsystemname]->outputs.d_ptr->scalars);
    std::map<string, double*>* scalar_states_map = &(d_ptr->subsystems_common_map[subsystemname]->states.d_ptr->scalars);
    if (scalar_output_map->count(subsystem_output) > 0) {
        double* ss_ouput_p = (*scalar_output_map)[subsystem_output];
        outputs.d_ptr->scalars[name] = ss_ouput_p;
        outputs.d_ptr->descriptions[name] = description;
    } else if (scalar_states_map->count(subsystem_output) > 0) {
        double* ss_ouput_p = (*scalar_states_map)[subsystem_output];
        outputs.d_ptr->scalars[name] = ss_ouput_p;
        outputs.d_ptr->descriptions[name] = description;
    }
}

} //End namespace pysim
