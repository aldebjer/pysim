#pragma once

#include "CythonSystemImpl.hpp"


void CythonSystemImpl::doStep(double time) {}
void CythonSystemImpl::doStoreStep(double time) {}
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
