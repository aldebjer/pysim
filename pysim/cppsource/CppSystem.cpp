#define _SCL_SECURE_NO_WARNINGS

#include "CppSystem.hpp"
#include "CommonSystemImpl_p.hpp"
#include "Variable_p.hpp"

#include <memory>
#include <boost/algorithm/string.hpp>
#include <vector>
#include <string>
#include <iostream>

using std::string;
using std::shared_ptr;

namespace pysim {

CppSystem::CppSystem(){
}

CppSystem::~CppSystem() {
}


///////////////////////////////////////
//
//      Cpp Interface
//
///////////////////////////////////////

void CppSystem::state(double* state, const char* stateName, double* der, const char* derName,const char* description) {
    string desc_string(description);
    string stateNameString(stateName);
    boost::algorithm::trim(stateNameString);
    states.d_ptr->scalars[stateNameString] = state;
    states.d_ptr->descriptions[stateName] = desc_string;

    string derNameString(derName);
    boost::algorithm::trim(derNameString);
    ders.d_ptr->scalars[derNameString] = der;
    ders.d_ptr->descriptions[derNameString] = desc_string;

    d_ptr->state_to_der_map_scalars[stateNameString] = derNameString;

}

void CppSystem::state(pysim::vector* state, const char* stateName, pysim::vector* der, const char* derName, const char* description) {
    string desc_string(description);

    string stateNameString(stateName);
    boost::algorithm::trim(stateNameString);
    states.d_ptr->vectors[stateNameString] = state;
    states.d_ptr->descriptions[stateName] = desc_string;

    string derNameString(derName);
    boost::algorithm::trim(derNameString);
    ders.d_ptr->vectors[derNameString] = der;
    ders.d_ptr->descriptions[derNameString] = desc_string;

    d_ptr->state_to_der_map_vectors[stateNameString] = derNameString;
}

void CppSystem::state(Eigen::MatrixXd* state, const char* stateName, Eigen::MatrixXd* der, const char* derName, const char* description) {
    string desc_string(description);

    string stateNameString(stateName);
    boost::algorithm::trim(stateNameString);
    states.d_ptr->matrices[stateNameString] = state;
    states.d_ptr->descriptions[stateName] = desc_string;

    string derNameString(derName);
    boost::algorithm::trim(derNameString);
    ders.d_ptr->matrices[derNameString] = der;
    ders.d_ptr->descriptions[derNameString] = desc_string;

    d_ptr->state_to_der_map_matrices[stateNameString] = derNameString;
}


void CppSystem::input(double* var, const char* name, const char* description) {
    std::string str(name);
    boost::algorithm::trim(str);
    inputs.d_ptr->scalars[str] = var;
    inputs.d_ptr->descriptions[str] = string(description);
}

void CppSystem::par(std::vector<double>* var, const char* name, const char* description) {
    //TODO:: Make parameter
    string str(name);
    boost::algorithm::trim(str);
    d_ptr->par_vectors[str] = var;
    d_ptr->par_descriptions[str] = string(description);
}

void CppSystem::par(std::vector<std::vector<double>>* var, const char* name, const char* description) {
    //TODO:: Make parameter
    string str(name);
    boost::algorithm::trim(str);
    d_ptr->par_matrices[str] = var;
    d_ptr->par_descriptions[str] = string(description);
}

void CppSystem::input(pysim::vector* vars, const char* name, const char* description) {
    string str(name);
    boost::algorithm::trim(str);
    inputs.d_ptr->vectors[str] = vars;
    inputs.d_ptr->descriptions[str] = string(description);
}

void CppSystem::par(boost::numeric::ublas::matrix<double>* var, const char* name, const char* description) {
    string str(name);
    boost::algorithm::trim(str);
    d_ptr->par_boost_matrices[str] = var;
    d_ptr->par_descriptions[str] = string(description);
}

void CppSystem::input(Eigen::MatrixXd* var, const char* name, const char* description) {
    string str(name);
    boost::algorithm::trim(str);
    inputs.d_ptr->matrices[str] = var;
    inputs.d_ptr->descriptions[str] = string(description);
}


void CppSystem::par(string* var, const char* name, const char* description) {
    string str(name);
    boost::algorithm::trim(str);
    d_ptr->par_strings[str] = var;
    d_ptr->par_descriptions[str] = string(description);
}

void CppSystem::par(std::map<string, double>* var, const char* name, const char* description) {
    string str(name);
    boost::algorithm::trim(str);
    d_ptr->par_maps[str] = var;
    d_ptr->par_descriptions[str] = string(description);
}

void CppSystem::par(std::map<string, std::vector<double>>* var, const char* name, const char* description) {
    string str(name);
    boost::algorithm::trim(str);
    d_ptr->par_vector_maps[str] = var;
    d_ptr->par_descriptions[str] = string(description);
}

void CppSystem::output(double* var, const char* name, const char* description) {
    string str(name);
    boost::algorithm::trim(str);
    outputs.d_ptr->scalars[str] = var;
    outputs.d_ptr->descriptions[str] = string(description);
}

void CppSystem::output(boost::numeric::ublas::vector<double>* vars, const char* name, const char* description) {
    string str(name);
    boost::algorithm::trim(str);
    outputs.d_ptr->vectors[str] = vars;
    outputs.d_ptr->descriptions[str] = string(description);
}

void CppSystem::output(Eigen::MatrixXd* var, const char* name, const char* description) {
    string str(name);
    boost::algorithm::trim(str);
    outputs.d_ptr->matrices[str] = var;
    outputs.d_ptr->descriptions[str] = string(description);
}

void CppSystem::setDiscrete(bool d) {
    d_ptr->isDiscrete = d;
}

void CppSystem::setNextUpdate(double t){
    d_ptr->nextUpdateTime = t;
}

}