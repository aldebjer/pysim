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
void CythonSystemImpl::doStoreStep(double time) {
    storestep_callback(sysp, time);
}
void CythonSystemImpl::copyoutputs() {}
void CythonSystemImpl::copystateoutputs() {}

std::vector<double*> CythonSystemImpl::getStatePointers() {
    return states;
}
std::vector<double*> CythonSystemImpl::getDerPointers() {
    return ders;
}

double CythonSystemImpl::getNextUpdateTime() { return 0; }
bool CythonSystemImpl::getDiscrete() { return false; }
bool CythonSystemImpl::do_comparison() { return false; }

void CythonSystemImpl::add_input_vector(std::string name, size_t length){
    d_ptr->input_vectors[name] = new pysim::vector(length);
    d_ptr->input_descriptions[name] = std::string("No Description"); //TODO add descriptions in call
}

