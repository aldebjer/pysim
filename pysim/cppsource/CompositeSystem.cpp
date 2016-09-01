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
    std::vector<std::string> subsystems_common_names;
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

/////////////////////////////////////////////////////////////////////
//
//         Python Interface
//
/////////////////////////////////////////////////////////////////////

void CompositeSystem::add_subsystem(CommonSystemImpl* subsystem, string name)
{
    d_ptr->subsystems_common_names.push_back(name);
    d_ptr->subsystems_common.push_back(subsystem);
}

void CompositeSystem::add_input_port(std::string name, size_t length)
{
}

void CompositeSystem::add_output_port(std::string name, size_t length)
{
}

} //End namespace pysim
