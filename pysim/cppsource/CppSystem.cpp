#define _SCL_SECURE_NO_WARNINGS

#include "CppSystem.hpp"
#include "StoreHandler.hpp"
#include <memory>
#include <boost/algorithm/string.hpp>
#include <vector>
#include <string>

using std::string;
using std::vector;
using std::shared_ptr;





CppSystem::CppSystem()
    :nextUpdateTime(0.0),
    isDiscrete(false),
    storeHandlerP(new StoreHandler()){
}

CppSystem::~CppSystem() {
};

////////////////////////////////////////
//
//        Inherited from System
//
////////////////////////////////////////

void CppSystem::doStoreStep(double time) {
    storeHandlerP->doStoreStep(time);
}

std::vector<double*> CppSystem::getStatePointers() {
    std::vector<double*> out;
    for (auto const& p : statemap) {
        out.push_back(p.second.stateValue);
     }

    for (auto const&p: state_boost_vectorsmap) {
        pysim::vector* v = p.second.stateValue;
        size_t size = v->size();
        for (size_t i = 0; i < size; ++i) {
            out.push_back(&(v->operator()(i)));
        }
    }
    return out;
}

std::vector<double*> CppSystem::getDerPointers() {
    std::vector<double*> out;
    for (auto const& p : statemap) {
        out.push_back(p.second.derValue);
    }

    for (auto const&p : state_boost_vectorsmap) {
        pysim::vector* v = p.second.derValue;
        size_t size = v->size();
        for (size_t i = 0; i < size; ++i) {
            out.push_back(&(v->operator()(i)));
        }
    }
    return out;
}


//////////////////////////////////////////////////////////////////////////////
//
//                        Python Interface
//
//////////////////////////////////////////////////////////////////////////////

// Store handling
////////////////

//Set the store interval
void CppSystem::setStoreInterval(double interval) {
    storeHandlerP->setStoreInterval(interval);
}

//Put the state, der, input or output named "name" in the vector of pointers 
//to be stored. If none with "name" is found the function raises an invalid_argument
//exception.
void  CppSystem::store(char* name) {
    if (statemap.count(name) == 1) {
        storeHandlerP->store_scalar(name, statemap[name].stateValue);
    } else if (dermap.count(name) == 1) {
        storeHandlerP->store_scalar(name, dermap[name].derValue);
    } else if (inputs.count(name) == 1) {
        storeHandlerP->store_scalar(name, inputs[name]);
    } else if (outputs.count(name) == 1) {
        storeHandlerP->store_scalar(name, outputs[name]);
    } else if (state_boost_vectorsmap.count(name) == 1) {
        storeHandlerP->store_vector(name, state_boost_vectorsmap[name].stateValue);
    } else if (der_boost_vectorsmap.count(name) == 1) {
        storeHandlerP->store_vector(name, der_boost_vectorsmap[name].derValue);
    } else if (input_boost_vectors.count(name) == 1) {
        storeHandlerP->store_vector(name, input_boost_vectors[name]);
    } else if (output_boost_vectors.count(name) == 1) {
        storeHandlerP->store_vector(name, output_boost_vectors[name]);
    } else {
        char errmsg[50];
        snprintf(errmsg,sizeof(errmsg), "Could not store: %s, no such variable", name);
        throw std::invalid_argument(errmsg);
    }
}

const std::vector<double>& CppSystem::getStoreVector(char* name) {
    return storeHandlerP->getStoreVector(name);
};

int CppSystem::getStoreSize() {
    return storeHandlerP->getStoreSize();
}

int CppSystem::getStoreColumns(char* name) {
    return storeHandlerP->getStoreColumns(name);
}

std::vector<string> CppSystem::getStoreNames() {
    return storeHandlerP->getStoreNames();
}

void CppSystem::fillWithStore(char* name, double* p, int rows, int columns) {
    storeHandlerP->fillWithStore(name, p, rows, columns);
}

void CppSystem::fillWithTime(double* p) {
    storeHandlerP->fillWithTime(p);
}

// Input handling
///////////////


void CppSystem::setInput(char* name, double value) {
    if (inputs.count(name) < 1) {
        char errmsg[50];
        snprintf(errmsg, 50, "Could not find: %s", name);
        throw std::invalid_argument(errmsg);
    }
    *(inputs.at(name)) = value;
}

void CppSystem::setInputVector(char* name, std::vector<double> value) {
    if (input_vectors.count(name) > 0) {
        std::vector<double> *inputv = input_vectors[name];
        inputv->clear();
        inputv->insert(inputv->begin(), value.begin(), value.end());
    } else if (input_boost_vectors.count(name) > 0) {
        auto bv = input_boost_vectors[name];
        if (bv->size() != value.size()) {
            std::string errstr = str(boost::format("Size of %1% is %2%") % name % bv->size());
            throw std::invalid_argument(errstr);
        }
        std::copy(value.begin(), value.end(), bv->begin());
    } else {
        std::string errstr = str(boost::format("Could not find: %1%") % name);
        throw std::invalid_argument(errstr);
    }

}

//Returns the names of both the std::vectors and the ublas::vectors
std::vector<std::string>  CppSystem::getInputVectorNames() {
    std::vector<std::string> std_names = getMapKeyStrings(input_vectors);
    std::vector<std::string> boost_names = getMapKeyStrings(input_boost_vectors);
    std_names.insert(std_names.end(), boost_names.begin(), boost_names.end());
    return std_names;
};

std::vector<std::string>  CppSystem::getInputMatrixNames() {
    std::vector<std::string> std_names = getMapKeyStrings(input_matrices);
    std::vector<std::string> boost_names = getMapKeyStrings(input_boost_matrices);
    std_names.insert(std_names.end(), boost_names.begin(), boost_names.end());
    return std_names;
};


std::vector<double> CppSystem::getInputVector(char* name) {
    if (input_vectors.count(name) > 0) {
        return *(input_vectors.at(name));
    } else if (input_boost_vectors.count(name) > 0) {
        auto bv = input_boost_vectors[name];
        std::vector<double> v(bv->size());
        std::copy(bv->begin(), bv->end(), v.begin());
        return v;
    } else {
        std::string errstr = str(boost::format("Could not find: %1%") % name);
        throw std::invalid_argument(errstr);
    }
}

std::vector<std::vector<double>> CppSystem::getInputMatrix(char* name) {
    std::vector<std::vector<double>> out;
    if (input_matrices.count(name) > 0) {
        out =  *input_matrices.at(name);
    }else if (input_boost_matrices.count(name) > 0) {
        using  boost::numeric::ublas::matrix;
        matrix<double>* mat = input_boost_matrices[name];

        size_t columns = mat->size2();
        for (matrix<double>::const_iterator1 rowiter = mat->begin1(); rowiter != mat->end1(); rowiter++) {
            std::vector<double> row(columns);
            std::copy(rowiter.begin(), rowiter.end(), row.begin());
            out.push_back(row);
        }
    } else {
        std::string errstr = str(boost::format("Could not find: %1%") % name);
        throw std::invalid_argument(errstr);
    }
    return out;
}

void CppSystem::setInputMatrix(char* name, std::vector<std::vector<double>> value) {
    using namespace boost::numeric::ublas;

    if (input_matrices.count(name) > 0) {
        *input_matrices.at(name) = value;
    } else if (input_boost_matrices.count(name) > 0){
        matrix<double> *inputm = input_boost_matrices.at(name);

        //Check number of rows
        if (value.size() != inputm->size1()) {
            std::string errstr = str(boost::format("Error: %1% shall contain %2% rows") % name % inputm->size1());
            throw std::invalid_argument(errstr);
        }

        //Check number of columns (for each row)
        size_t columns = inputm->size2();
        for (auto rowiter = value.cbegin(); rowiter != value.cend(); rowiter++) {
            if (rowiter->size() != columns) {
                std::string errstr = str(boost::format("Error: %1% shall contain %2% columns") % name % columns);
                throw std::invalid_argument(errstr);
            }
        }

        auto m1iter = inputm->begin1();
        for (auto rowiter = value.cbegin(); rowiter != value.cend(); rowiter++) {
            std::copy(rowiter->begin(), rowiter->end(), m1iter.begin());
            m1iter++;
        }
    }else{
        char errmsg[50];
        snprintf(errmsg, 50, "Could not find: %s", name);
        throw std::invalid_argument(errmsg);
    }
}

void CppSystem::setInputString(char* name, string value) {
    if (input_strings.count(name) < 1) {
        char errmsg[50];
        snprintf(errmsg, 50, "Could not find: %s", name);
        throw std::invalid_argument(errmsg);
    }
    *(input_strings.at(name)) = value;
}

std::map<std::string, double> CppSystem::getInputMap(char* name) {
    return getValue(name, input_maps);
}

void CppSystem::setInputMap(char* name, std::map<std::string, double> value) {
    if (input_maps.count(name) < 1) {
        char errmsg[50];
        snprintf(errmsg, 50, "Could not find: %s", name);
        throw std::invalid_argument(errmsg);
    }
    *input_maps.at(name)= value;
}

//Returns the names of both the std::vectors and the ublas::vectors
std::vector<std::string>  CppSystem::getOutputVectorNames() {
    std::vector<std::string> std_names = getMapKeyStrings(output_vectors);
    std::vector<std::string> boost_names = getMapKeyStrings(output_boost_vectors);
    std_names.insert(std_names.end(), boost_names.begin(), boost_names.end());
    return std_names;
};

std::vector<double> CppSystem::getOutputVector(char* name) {
    if (output_vectors.count(name) > 0) {
        return *(output_vectors.at(name));
    } else if (output_boost_vectors.count(name) > 0) {
        auto bv = output_boost_vectors[name];
        std::vector<double> v(bv->size());
        std::copy(bv->begin(), bv->end(), v.begin());
        return v;
    } else {
        std::string errstr = str(boost::format("Could not find: %1%") % name);
        throw std::invalid_argument(errstr);
    }
}

// State Handling
/////////////////

std::vector<std::string> CppSystem::getStateVectorNames() {
    std::vector<std::string> boostvectors = getMapKeyStrings(state_boost_vectorsmap);
    return boostvectors;
};

void CppSystem::setState(char* name, double value) {
    if (statemap.count(name) < 1) {
        char errmsg[50];
        snprintf(errmsg, 50, "Could not find: %s", name);
        throw std::invalid_argument(errmsg);
    }
    *(statemap.at(name).stateValue) = value;
}

void CppSystem::setStateVector(char* name, std::vector<double> value) {

    if (state_boost_vectorsmap.count(name) > 0) {
        auto bv = state_boost_vectorsmap[name].stateValue;
        if (bv->size() != value.size()) {
            std::string errstr = str(boost::format("Size of %1% is %2%") % name % bv->size());
            throw std::invalid_argument(errstr);
        }
        std::copy(value.begin(), value.end(), bv->begin());

    } else {
        std::string errstr = str(boost::format("Could not find: %1%") % name);
        throw std::invalid_argument(errstr);
    }
}

std::map<std::string, std::string> CppSystem::getStateDescriptionMap() {
    std::map<std::string, std::string> descMap;
    for (auto &desc : statemap) {
        descMap[desc.first] = desc.second.description;
    }
    for (auto &desc : state_boost_vectorsmap) {
        descMap[desc.first] = desc.second.description;
    }
    return descMap;
};

double CppSystem::getState(char* name){
    if (statemap.count(name) < 1) {
        std::string errstr = str(boost::format("Could not find: %1%") % name);
        throw std::invalid_argument(errstr);
    }
    return *(statemap.at(name).stateValue);
}


std::vector<double> CppSystem::getStateVector(char* name) {
    if (state_boost_vectorsmap.count(name) > 0) {
        pysim::vector* bv = state_boost_vectorsmap[name].stateValue;
        std::vector<double> v(bv->size());
        std::copy(bv->begin(), bv->end(), v.begin());
        return v;
    } else {
        std::string errstr = str(boost::format("Could not find: %1%") % name);
        throw std::invalid_argument(errstr);
    }
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
    stateItem.description = string(description);

    string stateNameString(stateName);
    boost::algorithm::trim(stateNameString);
    statemap[stateNameString] = stateItem;

    string derNameString(derName);
    boost::algorithm::trim(derNameString);
    dermap[derNameString] = stateItem;

}

void CppSystem::state(pysim::vector* state, const char* stateName, pysim::vector* der, const char* derName, const char* description) {
    StateType< pysim::vector*> stateItem;
    stateItem.stateValue = state;
    stateItem.derValue = der;
    stateItem.description = string(description);

    string stateNameString(stateName);
    boost::algorithm::trim(stateNameString);
    state_boost_vectorsmap[stateNameString] = stateItem;

    string derNameString(derName);
    boost::algorithm::trim(derNameString);
    der_boost_vectorsmap[derNameString] = stateItem;
}

void CppSystem::input(double* var, const char* name, const char* description) {
    std::string str(name);
    boost::algorithm::trim(str);
    inputs[str] = var;
    input_descriptions[str] = string(description);
}

void CppSystem::input(vector<double>* var, const char* name, const char* description) {
    string str(name);
    boost::algorithm::trim(str);
    input_vectors[str] = var;
    input_descriptions[str] = string(description);
}

void CppSystem::input(std::vector<std::vector<double>>* var, const char* name, const char* description) {
    string str(name);
    boost::algorithm::trim(str);
    input_matrices[str] = var;
    input_descriptions[str] = string(description);
}

void CppSystem::input(boost::numeric::ublas::vector<double>* vars, const char* name, const char* description) {
    string str(name);
    boost::algorithm::trim(str);
    input_boost_vectors[str] = vars;
    input_descriptions[str] = string(description);
}

void CppSystem::input(boost::numeric::ublas::matrix<double>* var, const char* name, const char* description) {
    string str(name);
    boost::algorithm::trim(str);
    input_boost_matrices[str] = var;
    input_descriptions[str] = string(description);
}

void CppSystem::input(string* var, const char* name, const char* description) {
    string str(name);
    boost::algorithm::trim(str);
    input_strings[str] = var;
    input_descriptions[str] = string(description);
}

void CppSystem::input(std::map<string, double>* var, const char* name, const char* description) {
    string str(name);
    boost::algorithm::trim(str);
    input_maps[str] = var;
    input_descriptions[str] = string(description);
}

void CppSystem::output(double* var, const char* name, const char* description) {
    string str(name);
    boost::algorithm::trim(str);
    outputs[str] = var;
    output_descriptions[str] = string(description);
}

void CppSystem::output(vector<double>* var, const char* name, const char* description) {
    string str(name);
    boost::algorithm::trim(str);
    output_vectors[str] = var;
    output_descriptions[str] = string(description);
}

void CppSystem::output(boost::numeric::ublas::vector<double>* vars, const char* name, const char* description) {
    string str(name);
    boost::algorithm::trim(str);
    output_boost_vectors[str] = vars;
    output_descriptions[str] = string(description);
}

void CppSystem::connect(char* outputname,
                        CppSystem* inputsys,
                        char* inputname) {
    using std::make_pair;

    if (inputsys->inputs.count(inputname) > 0) {
        if (outputs.count(outputname) == 1) {
            auto p = make_pair(outputs[outputname], inputsys->inputs[inputname]);
            outputvector.push_back(p);
        } else if (statemap.count(outputname) == 1) {
            auto p = make_pair(statemap[outputname].stateValue, inputsys->inputs[inputname]);
            stateOutputvector.push_back(p);
        } else {
            std::string errtxt("Could not find matching state or output to connect from");
            throw std::invalid_argument(errtxt);
        }
    } else if(inputsys->input_boost_vectors.count(inputname) > 0){
        if (output_boost_vectors.count(outputname) == 1) {
            auto p = make_pair(output_boost_vectors[outputname], inputsys->input_boost_vectors[inputname]);
            connectedBoostvectors.push_back(p);
        } else if (state_boost_vectorsmap.count(outputname) == 1) {
            auto p = make_pair(state_boost_vectorsmap[outputname].stateValue, inputsys->input_boost_vectors[inputname]);
            connectedBoostvectors.push_back(p);
        } else {
            std::string errtxt("Could not find matching state or output to connect from");
            throw std::invalid_argument(errtxt);
        }
    } else {
        throw std::invalid_argument("Could not find input to connect to");
    }
}

void CppSystem::add_compare_greater(char* comparename, double comparevalue) {
    using std::make_pair;

    if (outputs.count(comparename) == 1) {
        auto p = make_pair(outputs[comparename], comparevalue);
        compare_greater_vector.push_back(p);
    } else if (statemap.count(comparename) == 1) {
        auto p = make_pair(statemap[comparename].stateValue, comparevalue);
        compare_greater_vector.push_back(p);
    } else {
        std::string errtxt("Could not find state or output to use for comparison");
        throw std::invalid_argument(errtxt);
    }
}

void CppSystem::add_compare_smaller(char* comparename, double comparevalue) {
    using std::make_pair;

    if (outputs.count(comparename) == 1) {
        auto p = make_pair(outputs[comparename], comparevalue);
        compare_smaller_vector.push_back(p);
    } else if (statemap.count(comparename) == 1) {
        auto p = make_pair(statemap[comparename].stateValue, comparevalue);
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

