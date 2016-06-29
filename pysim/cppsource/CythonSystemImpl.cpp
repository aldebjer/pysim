#include <map>

#include "PysimTypes.hpp"
#include "CythonSystemImpl.hpp"
#include "../cythonsystem_api.h"
#include "CommonSystemImpl_p.hpp"

#include <iostream>
#include <string>

using std::string;


CythonSystemImpl::CythonSystemImpl()
{
    import_pysim__cythonsystem();
}

void CythonSystemImpl::doStep(double time) {
    step_callback(sysp,time);
}

double CythonSystemImpl::getNextUpdateTime() { return 0; }
bool CythonSystemImpl::getDiscrete() { return false; }
bool CythonSystemImpl::do_comparison() { return false; }

void CythonSystemImpl::add_input_vector(std::string name, size_t length){
    d_ptr->input_vectors[name] = new pysim::vector(length);
    d_ptr->input_descriptions[name] = std::string("No Description"); //TODO add descriptions in call
}

void CythonSystemImpl::add_output_vector(std::string name, size_t length) {
    d_ptr->output_vectors[name] = new pysim::vector(length);
    d_ptr->output_descriptions[name] = std::string("No Description"); //TODO add descriptions in call
}

void CythonSystemImpl::add_state_vector(std::string statename, std::string dername, size_t length) {
    StateType<pysim::vector* > state;
    state.stateValue = new pysim::vector(length);
    state.derValue = new pysim::vector(length);
    d_ptr->state_vectors[statename] = state;
    d_ptr->der_vectors[dername] = state;
    d_ptr->output_descriptions[statename] = std::string("No Description"); //TODO add descriptions in call
}

