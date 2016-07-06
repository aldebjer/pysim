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



CppSystem::CppSystem()
    :nextUpdateTime(0.0),
    isDiscrete(false){
}

CppSystem::~CppSystem() {
};


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

void CppSystem::output(vector<double>* var, const char* name, const char* description) {
    std::cout << "depreciated function, std::vector output" << std::endl;
    //string str(name);
    //boost::algorithm::trim(str);
    //output_vectors[str] = var;
    //output_descriptions[str] = string(description);
}

void CppSystem::output(boost::numeric::ublas::vector<double>* vars, const char* name, const char* description) {
    string str(name);
    boost::algorithm::trim(str);
    d_ptr->output_vectors[str] = vars;
    d_ptr->output_descriptions[str] = string(description);
}



void CppSystem::add_compare_greater(char* comparename, double comparevalue) {
    using std::make_pair;

    if (d_ptr->output_scalars.count(comparename) == 1) {
        auto p = make_pair(d_ptr->output_scalars[comparename], comparevalue);
        compare_greater_vector.push_back(p);
    } else if (d_ptr->state_scalars.count(comparename) == 1) {
        auto p = make_pair(d_ptr->state_scalars[comparename].stateValue, comparevalue);
        compare_greater_vector.push_back(p);
    } else {
        std::string errtxt("Could not find state or output to use for comparison");
        throw std::invalid_argument(errtxt);
    }
}

void CppSystem::add_compare_smaller(char* comparename, double comparevalue) {
    using std::make_pair;

    if (d_ptr->output_scalars.count(comparename) == 1) {
        auto p = make_pair(d_ptr->output_scalars[comparename], comparevalue);
        compare_smaller_vector.push_back(p);
    } else if (d_ptr->state_scalars.count(comparename) == 1) {
        auto p = make_pair(d_ptr->state_scalars[comparename].stateValue, comparevalue);
        compare_smaller_vector.push_back(p);
    } else {
        std::string errtxt("Could not find state or output to use for comparison");
        throw std::invalid_argument(errtxt);
    }
}


bool CppSystem::do_comparison() {
    bool is_greater = false;
    auto compare_greater_pairs = compare_greater_vector.begin();
    while (compare_greater_pairs != compare_greater_vector.end()) {
        if (*(compare_greater_pairs->first) > compare_greater_pairs->second) {
            is_greater = true;
            compare_greater_vector.erase(compare_greater_pairs);
        } else {
            compare_greater_pairs++;
        }
    }

    bool is_smaller = false;
    auto compare_smaller_pairs = compare_smaller_vector.begin();
    while (compare_smaller_pairs != compare_smaller_vector.end()) {
        if (*(compare_smaller_pairs->first) < compare_smaller_pairs->second) {
            is_smaller = true;
            compare_smaller_vector.erase(compare_smaller_pairs);
        } else {
            compare_smaller_pairs++;
        }
    }

    return is_greater || is_smaller;
}

double CppSystem::getNextUpdateTime() {
    return nextUpdateTime;
}
bool CppSystem::getDiscrete() {
    return isDiscrete;
}


