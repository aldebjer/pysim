#pragma once
#include <vector>
#include <map>

#include "CommonSystemImpl.hpp"
#include "Variable.hpp"

#include "StoreHandler.hpp"
#include "PysimTypes.hpp"


namespace pysim {

template <class T>
struct StateType {
    T stateValue;
    T derValue;
    std::string description;
};

struct CommonSystemImplPrivate {

    std::map<std::string, std::string> state_to_der_map_scalars;
    std::map<std::string, std::string> state_to_der_map_vectors;

    std::map<std::string, std::string*> par_strings;
    std::map<std::string, std::map<std::string, double>* > par_maps;
    std::map<std::string, std::map<std::string, std::vector<double>>* > par_vector_maps;
    std::map<std::string, std::vector<double>* > par_vectors;
    std::map<std::string, std::vector<std::vector<double>>* > par_matrices;
    std::map<std::string, pysim::matrix* > par_boost_matrices;
    std::map<std::string, std::string> par_descriptions;

    StoreHandler storeHandler;

    bool isDiscrete {false};
    double nextUpdateTime{0.0};

    std::vector<std::pair<double*, double > > compare_greater_vector;
    std::vector<std::pair<double*, double > > compare_smaller_vector;

};

}

