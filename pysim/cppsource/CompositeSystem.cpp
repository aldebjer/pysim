#include "CompositeSystem.hpp"

#include <map>

#include "CommonSystemImpl_p.hpp"

#include "Variable_p.hpp"

#include "PysimTypes.hpp"
#include "CompositeSystem.hpp"

#include <iostream>
#include <string>

using std::string;

namespace pysim{

struct CompositeSystemPrivate {
    std::vector<CommonSystemImpl*> subsystems_common;
    std::map<std::string, CommonSystemImpl*> subsystems_common_map;

    std::vector<std::pair<double*, double* > > connected_scalars;
    std::vector<std::pair<pysim::vector*, pysim::vector* > > connected_vectors;
};


CompositeSystem::CompositeSystem()
{
}
CompositeSystem::~CompositeSystem()
{
}

//////////////////////////////////////////////////////////////////////////
//      Inherited from Simulatable System
//////////////////////////////////////////////////////////////////////////

std::vector<double*> CompositeSystem::getStatePointers() {
    std::vector<double*> states;
    for (CommonSystemImpl* s : d_ptr->subsystems_common) {
        std::vector<double*> subs_states = s->getStatePointers();
        std::copy(subs_states.begin(), subs_states.end(), std::back_inserter(states));
    }
    return states;
};

std::vector<double*> CompositeSystem::getDerPointers() {
    std::vector<double*> ders;
    for (CommonSystemImpl* s : d_ptr->subsystems_common) {
        std::vector<double*> subs_ders = s->getDerPointers();
        std::copy(subs_ders.begin(), subs_ders.end(), std::back_inserter(ders));
    }
    return ders;
};


void CompositeSystem::preSim()
{
    for (CommonSystemImpl* s : d_ptr->subsystems_common) {
        s->preSim();
    }
}

void CompositeSystem::doStep(double time)
{
    for (CommonSystemImpl* s : d_ptr->subsystems_common) {
        s->doStep(time);
    }
}

double CompositeSystem::getNextUpdateTime() 
{
    return 0;
}

bool CompositeSystem::do_comparison()
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
void CompositeSystem::connect(char* outputname,
    CommonSystemImpl* inputsys,
    char* inputname) {
    using std::make_pair;

    if (inputsys->inputs.d_ptr->scalars.count(inputname) > 0) {
        if (outputs.d_ptr->scalars.count(outputname) == 1) {
            auto p = make_pair(outputs.d_ptr->scalars[outputname], inputsys->inputs.d_ptr->scalars[inputname]);
            d_ptr->connected_scalars.push_back(p);
        } else {
            std::string errtxt("Could not find matching state or output to connect from");
            throw std::invalid_argument(errtxt);
        }
    } else if (inputsys->inputs.d_ptr->vectors.count(inputname) > 0) {
        if (outputs.d_ptr->vectors.count(outputname) == 1) {
            auto p = make_pair(outputs.d_ptr->vectors[outputname], inputsys->inputs.d_ptr->vectors[inputname]);
            d_ptr->connected_vectors.push_back(p);
        }else {
            std::string errtxt("Could not find matching state or output to connect from");
            throw std::invalid_argument(errtxt);
        }
    } else {
        throw std::invalid_argument("Could not find input to connect to");
    }
}


void CompositeSystem::copyoutputs() {
    for (auto vi = d_ptr->connected_scalars.cbegin(); vi != d_ptr->connected_scalars.cend(); ++vi) {
        *(vi->second) = *(vi->first);
    }

    for (auto vi = d_ptr->connected_vectors.cbegin(); vi != d_ptr->connected_vectors.cend(); ++vi) {
        *(vi->second) = *(vi->first);
    }
}

/////////////////////////////////////////////////////////////////////
//
//         Python Interface
//
/////////////////////////////////////////////////////////////////////

void CompositeSystem::add_subsystem(CommonSystemImpl* subsystem, string name)
{
    d_ptr->subsystems_common_map[name] = subsystem;
    d_ptr->subsystems_common.push_back(subsystem);
}

void CompositeSystem::add_input_port(string name, string subsystemname, string subsystem_input, string description)
{
    double* ss_input_p = d_ptr->subsystems_common_map[subsystemname]->inputs.d_ptr->scalars[subsystem_input];

    inputs.d_ptr->scalars[name] = ss_input_p;
    inputs.d_ptr->descriptions[name] = description;
}

void CompositeSystem::add_output_port(string name, string subsystemname, string subsystem_output, string description)
{
    double* ss_ouput_p = d_ptr->subsystems_common_map[subsystemname]->outputs.d_ptr->scalars[subsystem_output];

    outputs.d_ptr->scalars[name] = ss_ouput_p;
    outputs.d_ptr->descriptions[name] = description;
}

} //End namespace pysim
