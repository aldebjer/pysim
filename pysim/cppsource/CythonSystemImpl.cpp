#include "CommonSystemImpl_p.hpp"

#include "Variable_p.hpp"

#include "PysimTypes.hpp"
#include "CythonSystemImpl.hpp"
#include "../cythonsystem_api.h"

#include <iostream>
#include <string>

using std::string;

namespace pysim{

CythonSystemImpl::CythonSystemImpl()
{
    import_pysim__cythonsystem();
}

void CythonSystemImpl::doStep(double time) {
    step_callback(sysp,time);
}

double CythonSystemImpl::getNextUpdateTime() { return 0; }

bool CythonSystemImpl::do_comparison() { return false; }

void CythonSystemImpl::add_input(std::string name, size_t length){
    if (length == 1) {
        inputs.d_ptr->scalars[name] = new double(0);
    } else {
        inputs.d_ptr->vectors[name] = new pysim::vector(length,0.0);
        inputs.d_ptr->descriptions[name] = std::string("No Description"); //TODO add descriptions in call
    }
}

void CythonSystemImpl::add_output(std::string name, size_t length) {
    if (length == 1) {
        outputs.d_ptr->scalars[name] = new double(0);
    } else {
        outputs.d_ptr->vectors[name] = new pysim::vector(length);
    }
    outputs.d_ptr->descriptions[name] = std::string("No Description"); //TODO add descriptions in call
}

void CythonSystemImpl::add_state(std::string statename, std::string dername, size_t length) {
    if (length == 1) {
        states.d_ptr->scalars[statename] = new double(0.0);
        ders.d_ptr->scalars[dername] = new double(0.0);
        d_ptr->state_to_der_map_scalars[statename] = dername;
    } else {
        states.d_ptr->vectors[statename] = new pysim::vector(length);
        ders.d_ptr->vectors[dername] = new pysim::vector(length);
        d_ptr->state_to_der_map_vectors[statename] = dername;
    }
    states.d_ptr->descriptions[statename] = std::string("No Description"); //TODO add descriptions in call'
    ders.d_ptr->descriptions[statename] = std::string("No Description"); //TODO add descriptions in call
}

}
