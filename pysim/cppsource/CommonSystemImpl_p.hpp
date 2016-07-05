#pragma once
#include <vector>
#include <map>

#include "PysimTypes.hpp"

template <class T>
struct StateType {
    T stateValue;
    T derValue;
    std::string description;
};

struct CommonSystemImplPrivate {

    std::map<std::string, StateType<double* >> state_scalars;
    std::map<std::string, StateType<pysim::vector* > > state_vectors;

    std::map<std::string, StateType<double* >> der_scalars;
    std::map<std::string, StateType<pysim::vector* > > der_vectors;

    std::map<std::string, std::string> state_descriptions;
    std::map<std::string, std::string> der_descriptions;

    std::map<std::string, std::string*> par_strings;
    std::map<std::string, std::string> par_descriptions;

    std::map<std::string, double* > input_scalars;
    std::map<std::string, pysim::vector* > input_vectors;
    std::map<std::string, std::string> input_descriptions;

    std::map<std::string, double* > output_scalars;
    std::map<std::string, pysim::vector* > output_vectors;
    std::map<std::string, std::string> output_descriptions;

    std::vector<std::pair<double*, double* > > connected_scalars;
    std::vector<std::pair<pysim::vector*, pysim::vector* > > connected_vectors;

    std::vector<std::pair<double*, double* > > connected_scalar_states_;
    std::vector<std::pair<pysim::vector*, pysim::vector* > > connected_vector_states;

    StoreHandler storeHandler;
};


