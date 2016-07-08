#define _SCL_SECURE_NO_WARNINGS

#include "CppSystem.hpp"
#include "CommonSystemImpl_p.hpp"

#include <memory>
#include <boost/algorithm/string.hpp>
#include <vector>
#include <string>
#include <iostream>

using std::string;
using std::vector;
using std::shared_ptr;



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
    StateType<double*> stateItem;
    stateItem.stateValue = state;
    stateItem.derValue = der;
    string desc_string(description);
    stateItem.description = desc_string;
    string stateNameString(stateName);
    boost::algorithm::trim(stateNameString);
    d_ptr->state_scalars[stateNameString] = stateItem;
    d_ptr->state_descriptions[stateName] = desc_string;

    string derNameString(derName);
    boost::algorithm::trim(derNameString);
    d_ptr->der_scalars[derNameString] = stateItem;
    d_ptr->der_descriptions[derNameString] = desc_string;

}

void CppSystem::state(pysim::vector* state, const char* stateName, pysim::vector* der, const char* derName, const char* description) {
    StateType< pysim::vector*> stateItem;
    string desc_string(description);

    stateItem.stateValue = state;
    stateItem.derValue = der;
    stateItem.description = desc_string;

    string stateNameString(stateName);
    boost::algorithm::trim(stateNameString);
    d_ptr->state_vectors[stateNameString] = stateItem;
    d_ptr->state_descriptions[stateName] = desc_string;

    string derNameString(derName);
    boost::algorithm::trim(derNameString);
    d_ptr->der_vectors[derNameString] = stateItem;
    d_ptr->der_descriptions[derNameString] = desc_string;
}

void CppSystem::input(double* var, const char* name, const char* description) {
    std::string str(name);
    boost::algorithm::trim(str);
    d_ptr->input_scalars[str] = var;
    d_ptr->input_descriptions[str] = string(description);
}

void CppSystem::input(vector<double>* var, const char* name, const char* description) {
    //TODO:: Make parameter
    //string str(name);
    //boost::algorithm::trim(str);
    //input_vectors[str] = var;
    //d_ptr->input_descriptions[str] = string(description);
}

void CppSystem::input(std::vector<std::vector<double>>* var, const char* name, const char* description) {
    //TODO:: Make parameter
    //string str(name);
    //boost::algorithm::trim(str);
    //input_matrices[str] = var;
    //d_ptr->input_descriptions[str] = string(description);
}

void CppSystem::input(boost::numeric::ublas::vector<double>* vars, const char* name, const char* description) {
    string str(name);
    boost::algorithm::trim(str);
    d_ptr->input_vectors[str] = vars;
    d_ptr->input_descriptions[str] = string(description);
}

void CppSystem::input(boost::numeric::ublas::matrix<double>* var, const char* name, const char* description) {
    //TODO:: Make parameter
    string str(name);
    boost::algorithm::trim(str);
    d_ptr->par_boost_matrices[str] = var;
    d_ptr->par_descriptions[str] = string(description);
}

void CppSystem::input(string* var, const char* name, const char* description) {
    string str(name);
    boost::algorithm::trim(str);
    d_ptr->par_strings[str] = var;
    d_ptr->par_descriptions[str] = string(description);
}

void CppSystem::input(std::map<string, double>* var, const char* name, const char* description) {
    string str(name);
    boost::algorithm::trim(str);
    d_ptr->par_maps[str] = var;
    d_ptr->par_descriptions[str] = string(description);
}

void CppSystem::output(double* var, const char* name, const char* description) {
    string str(name);
    boost::algorithm::trim(str);
    d_ptr->output_scalars[str] = var;
    d_ptr->output_descriptions[str] = string(description);
}

void CppSystem::output(boost::numeric::ublas::vector<double>* vars, const char* name, const char* description) {
    string str(name);
    boost::algorithm::trim(str);
    d_ptr->output_vectors[str] = vars;
    d_ptr->output_descriptions[str] = string(description);
}

void CppSystem::setDiscrete(bool d) {
    d_ptr->isDiscrete = d;
}

void CppSystem::setNextUpdate(double t){
    d_ptr->nextUpdateTime = t;
}

