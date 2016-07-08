#include <map>

#include "CommonSystemImpl.hpp"
#include "CommonSystemImpl_p.hpp"

#include <boost/algorithm/string.hpp>
#include <iostream>
#include <string>
#include <boost/format.hpp>

using std::string;


CommonSystemImpl::CommonSystemImpl() :
    d_ptr(new CommonSystemImplPrivate())
{
}

CommonSystemImpl::~CommonSystemImpl(){
}

//////////////////////////////
//
//     Parameter handling
//
//////////////////////////////
std::vector<std::string> CommonSystemImpl::getParStringNames() {
    std::vector<std::string> names;
    for (auto i = d_ptr->par_strings.cbegin(); i != d_ptr->par_strings.cend(); ++i) {
        names.push_back(i->first);
    }
    return names;
}

std::vector<std::string> CommonSystemImpl::getParMatrixNames() {
    std::vector<std::string> names;
    for (auto i = d_ptr->par_matrices.cbegin(); i != d_ptr->par_matrices.cend(); ++i) {
        names.push_back(i->first);
    }
    for (auto i = d_ptr->par_boost_matrices.cbegin(); i != d_ptr->par_boost_matrices.cend(); ++i) {
        names.push_back(i->first);
    }
    return names;
}

std::vector<std::string> CommonSystemImpl::getParMapNames() {
    std::vector<std::string> names;
    for (auto i = d_ptr->par_maps.cbegin(); i != d_ptr->par_maps.cend(); ++i) {
        names.push_back(i->first);
    }
    return names;
}


std::string CommonSystemImpl::getParString(char* name) {
    if (d_ptr->par_strings.count(name) < 1) {
        std::string errstr = str(boost::format("Could not find: %1%") % name);
        throw std::invalid_argument(errstr);
    }
    std::string s(*d_ptr->par_strings.at(name));
    return s;
}
void CommonSystemImpl::setParString(char* name, std::string value) {
    if (d_ptr->par_strings.count(name) < 1) {
        char errmsg[50];
        snprintf(errmsg, 50, "Could not find: %s", name);
        throw std::invalid_argument(errmsg);
    }
    *(d_ptr->par_strings.at(name)) = value;
}

std::vector<std::vector<double>> CommonSystemImpl::getParMatrix(char* name) {
    std::vector<std::vector<double>> out;
    if (d_ptr->par_matrices.count(name) > 0) {
        out = *d_ptr->par_matrices.at(name);
    } else if (d_ptr->par_boost_matrices.count(name) > 0) {
        using  boost::numeric::ublas::matrix;
        matrix<double>* mat = d_ptr->par_boost_matrices[name];

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
void CommonSystemImpl::setParMatrix(char* name, std::vector<std::vector<double>> value) {
    using namespace boost::numeric::ublas;

    if (d_ptr->par_matrices.count(name) > 0) {
        *d_ptr->par_matrices.at(name) = value;
    } else if (d_ptr->par_boost_matrices.count(name) > 0) {
        matrix<double> *inputm = d_ptr->par_boost_matrices.at(name);

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
    } else {
        char errmsg[50];
        snprintf(errmsg, 50, "Could not find: %s", name);
        throw std::invalid_argument(errmsg);
    }
}

std::map<std::string, double> CommonSystemImpl::getParMap(char* name) {
    if (d_ptr->par_maps.count(name) < 1) {
        std::string errstr = str(boost::format("Could not find: %1%") % name);
        throw std::invalid_argument(errstr);
    }
    return *(d_ptr->par_maps.at(name));
}

void CommonSystemImpl::setParMap(char* name, std::map<std::string, double> value) {
    if (d_ptr->par_maps.count(name) < 1) {
        char errmsg[50];
        snprintf(errmsg, 50, "Could not find: %s", name);
        throw std::invalid_argument(errmsg);
    }
    *d_ptr->par_maps.at(name) = value;
}

std::map<std::string, std::string> CommonSystemImpl::getParDescriptionMap() {
    return d_ptr->par_descriptions;
}

//////////////////////////////
//
//     Input handling
//
//////////////////////////////

std::map<std::string, std::string> CommonSystemImpl::getInputDescriptionMap(){
    return d_ptr->input_descriptions;
}

std::vector<std::string> CommonSystemImpl::getScalarInputNames() {
    std::vector<std::string> names;
    for (auto i = d_ptr->input_scalars.cbegin(); i != d_ptr->input_scalars.cend(); ++i) {
        names.push_back(i->first);
    }
    return names;
}

std::vector<std::string>  CommonSystemImpl::getInputVectorNames() {
    std::vector<std::string> names;
    for (auto i = d_ptr->input_vectors.cbegin(); i != d_ptr->input_vectors.cend(); ++i) {
        names.push_back(i->first);
    }
    return names;
};

void CommonSystemImpl::setScalarInput(char* name, double value) {
    if (d_ptr->input_scalars.count(name) < 1) {
        char errmsg[50];
        snprintf(errmsg, 50, "Could not find: %s", name);
        throw std::invalid_argument(errmsg);
    }
    *(d_ptr->input_scalars.at(name)) = value;
}

void CommonSystemImpl::setInputVector(char* name, std::vector<double> value) {
    if (d_ptr->input_vectors.count(name) > 0) {
        auto bv = d_ptr->input_vectors[name];
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

double CommonSystemImpl::getScalarInput(char* name) {
    if (d_ptr->input_scalars.count(name) < 1) {
        std::string errstr = str(boost::format("Could not find: %1%") % name);
        throw std::invalid_argument(errstr);
    }
    return *(d_ptr->input_scalars.at(name));
};

std::vector<double> CommonSystemImpl::getInputVector(char* name) {
    if (d_ptr->input_vectors.count(name) > 0) {
        auto bv = d_ptr->input_vectors[name];
        std::vector<double> v(bv->size());
        std::copy(bv->begin(), bv->end(), v.begin());
        return v;
    } else {
        std::string errstr = str(boost::format("Could not find: %1%") % name);
        throw std::invalid_argument(errstr);
    }
}

//////////////////////////////
//
//     Output handling
//
//////////////////////////////
std::vector<std::string> CommonSystemImpl::getScalarOutputNames() {
    std::vector<std::string> names;
    for (auto i = d_ptr->output_scalars.cbegin(); i != d_ptr->output_scalars.cend(); ++i) {
        names.push_back(i->first);
    }
    return names;
};

std::vector<std::string> CommonSystemImpl::getOutputVectorNames() {
    std::vector<std::string> names;
    for (auto i = d_ptr->output_vectors.cbegin(); i != d_ptr->output_vectors.cend(); ++i) {
        names.push_back(i->first);
    }
    return names;
}

double CommonSystemImpl::getScalarOutput(char* name) {
    if (d_ptr->output_scalars.count(name) < 1) {

        std::string errstr = str(boost::format("Could not find: %1%") % name);
        throw std::invalid_argument(errstr);
    }
    return *(d_ptr->output_scalars.at(name));
};

std::vector<double> CommonSystemImpl::getOutputVector(char* name) {
    if (d_ptr->output_vectors.count(name) > 0) {
        auto bv = d_ptr->output_vectors[name];
        std::vector<double> v(bv->size());
        std::copy(bv->begin(), bv->end(), v.begin());
        return v;
    } else {
        std::string errstr = str(boost::format("Could not find: %1%") % name);
        throw std::invalid_argument(errstr);
    }
}

void CommonSystemImpl::setScalarOutput(char* name, double value) {
    if (d_ptr->output_scalars.count(name) < 1) {
        char errmsg[50];
        snprintf(errmsg, 50, "Could not find: %s", name);
        throw std::invalid_argument(errmsg);
    }
    *(d_ptr->output_scalars.at(name)) = value;
}

void CommonSystemImpl::setOutputVector(char* name, std::vector<double> value) {
    if (d_ptr->output_vectors.count(name) > 0) {
        auto bv = d_ptr->output_vectors[name];
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

std::map<std::string, std::string> CommonSystemImpl::getOutputDescriptionMap() {
    return  d_ptr->output_descriptions;
}

//////////////////////////////
//
//     State handling
//
//////////////////////////////
std::vector<std::string> CommonSystemImpl::getScalarStateNames() {
    std::vector<std::string> names;
    for (auto i = d_ptr->state_scalars.cbegin(); i != d_ptr->state_scalars.cend(); ++i) {
        names.push_back(i->first);
    }
    return names;
};

std::vector<std::string> CommonSystemImpl::getStateVectorNames() {
    std::vector<std::string> names;
    for (auto i = d_ptr->state_vectors.cbegin(); i != d_ptr->state_vectors.cend(); ++i) {
        names.push_back(i->first);
    }
    return names;
}

double CommonSystemImpl::getScalarState(char* name) {
    if (d_ptr->state_scalars.count(name) < 1) {

        std::string errstr = str(boost::format("Could not find: %1%") % name);
        throw std::invalid_argument(errstr);
    }
    return *(d_ptr->state_scalars.at(name).stateValue);
};

std::vector<double> CommonSystemImpl::getStateVector(char* name) {
    if (d_ptr->state_vectors.count(name) > 0) {
        auto bv = d_ptr->state_vectors[name].stateValue;
        std::vector<double> v(bv->size());
        std::copy(bv->begin(), bv->end(), v.begin());
        return v;
    } else {
        std::string errstr = str(boost::format("Could not find: %1%") % name);
        throw std::invalid_argument(errstr);
    }
}

void CommonSystemImpl::setStateVector(char* name, std::vector<double> value) {
    if (d_ptr->state_vectors.count(name) > 0) {
        auto bv = d_ptr->state_vectors[name].stateValue;
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

void CommonSystemImpl::setScalarState(char* name, double value) {
    if (d_ptr->state_scalars.count(name) < 1) {
        char errmsg[50];
        snprintf(errmsg, 50, "Could not find: %s", name);
        throw std::invalid_argument(errmsg);
    }
    *(d_ptr->state_scalars.at(name).stateValue) = value;
}

std::map<std::string, std::string> CommonSystemImpl::getStateDescriptionMap() {
    return  d_ptr->state_descriptions;
}

//////////////////////////////
//
//     Der handling
//
//////////////////////////////
std::vector<std::string> CommonSystemImpl::getScalarDerNames() {
    std::vector<std::string> names;
    for (auto i = d_ptr->der_scalars.cbegin(); i != d_ptr->der_scalars.cend(); ++i) {
        names.push_back(i->first);
    }
    return names;
};

std::vector<std::string> CommonSystemImpl::getDerVectorNames() {
    std::vector<std::string> names;
    for (auto i = d_ptr->der_vectors.cbegin(); i != d_ptr->der_vectors.cend(); ++i) {
        names.push_back(i->first);
    }
    return names;
}

double CommonSystemImpl::getScalarDer(char* name) {
    if (d_ptr->der_scalars.count(name) < 1) {

        std::string errstr = str(boost::format("Could not find: %1%") % name);
        throw std::invalid_argument(errstr);
    }
    return *(d_ptr->der_scalars.at(name).derValue);
};

std::vector<double> CommonSystemImpl::getDerVector(char* name) {
    if (d_ptr->der_vectors.count(name) > 0) {
        auto bv = d_ptr->state_vectors[name].derValue;
        std::vector<double> v(bv->size());
        std::copy(bv->begin(), bv->end(), v.begin());
        return v;
    } else {
        std::string errstr = str(boost::format("Could not find: %1%") % name);
        throw std::invalid_argument(errstr);
    }
}

void CommonSystemImpl::setDerVector(char* name, std::vector<double> value) {
    if (d_ptr->der_vectors.count(name) > 0) {
        auto bv = d_ptr->der_vectors[name].derValue;
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

void CommonSystemImpl::setScalarDer(char* name, double value) {
    if (d_ptr->der_scalars.count(name) < 1) {
        char errmsg[50];
        snprintf(errmsg, 50, "Could not find: %s", name);
        throw std::invalid_argument(errmsg);
    }
    *(d_ptr->der_scalars.at(name).derValue) = value;
}

std::map<std::string, std::string> CommonSystemImpl::getDerDescriptionMap() {
    return  d_ptr->der_descriptions;
}

////////////////////////////////////
//
//       Connections
//
////////////////////////////////////
void CommonSystemImpl::connect(char* outputname,
    CommonSystemImpl* inputsys,
    char* inputname) {
    using std::make_pair;

    if (inputsys->d_ptr->input_scalars.count(inputname) > 0) {
        if (d_ptr->output_scalars.count(outputname) == 1) {
            auto p = make_pair(d_ptr->output_scalars[outputname], inputsys->d_ptr->input_scalars[inputname]);
            d_ptr->connected_scalars.push_back(p);
        } else if (d_ptr->state_scalars.count(outputname) == 1) {
            auto p = make_pair(d_ptr->state_scalars[outputname].stateValue, inputsys->d_ptr->input_scalars[inputname]);
            d_ptr->connected_scalar_states_.push_back(p);
        } else {
            std::string errtxt("Could not find matching state or output to connect from");
            throw std::invalid_argument(errtxt);
        }
    } else if (inputsys->d_ptr->input_vectors.count(inputname) > 0) {
        if (d_ptr->output_vectors.count(outputname) == 1) {
            auto p = make_pair(d_ptr->output_vectors[outputname], inputsys->d_ptr->input_vectors[inputname]);
            d_ptr->connected_vectors.push_back(p);
        } else if (d_ptr->state_vectors.count(outputname) == 1) {
            auto p = make_pair(d_ptr->state_vectors[outputname].stateValue, inputsys->d_ptr->input_vectors[inputname]);
            d_ptr->connected_vector_states.push_back(p);
        } else {
            std::string errtxt("Could not find matching state or output to connect from");
            throw std::invalid_argument(errtxt);
        }
    } else {
        throw std::invalid_argument("Could not find input to connect to");
    }
}


void CommonSystemImpl::copyoutputs() {
    for (auto vi = d_ptr->connected_scalars.cbegin(); vi != d_ptr->connected_scalars.cend(); ++vi) {
        *(vi->second) = *(vi->first);
    }

    for (auto vi = d_ptr->connected_vectors.cbegin(); vi != d_ptr->connected_vectors.cend(); ++vi) {
        *(vi->second) = *(vi->first);
    }
}

void CommonSystemImpl::copystateoutputs() {
    for (auto vi = d_ptr->connected_scalar_states_.cbegin(); vi != d_ptr->connected_scalar_states_.cend(); ++vi) {
        *(vi->second) = *(vi->first);
    }

    for (auto vi = d_ptr->connected_vector_states.cbegin(); vi != d_ptr->connected_vector_states.cend(); ++vi) {
        *(vi->second) = *(vi->first);
    }
}


std::vector<double*> CommonSystemImpl::getStatePointers() {
    std::vector<double*> out;
    for (auto const& p : d_ptr->state_scalars) {
        out.push_back(p.second.stateValue);
    }

    for (auto const&p : d_ptr->state_vectors) {
        pysim::vector* v = p.second.stateValue;
        size_t size = v->size();
        for (size_t i = 0; i < size; ++i) {
            out.push_back(&(v->operator()(i)));
        }
    }
    return out;
}

std::vector<double*> CommonSystemImpl::getDerPointers() {
    std::vector<double*> out;
    for (auto const& p : d_ptr->state_scalars) {
        out.push_back(p.second.derValue);
    }

    for (auto const&p : d_ptr->state_vectors) {
        pysim::vector* v = p.second.derValue;
        size_t size = v->size();
        for (size_t i = 0; i < size; ++i) {
            out.push_back(&(v->operator()(i)));
        }
    }
    return out;
}

void CommonSystemImpl::doStoreStep(double time) {
    d_ptr->storeHandler.doStoreStep(time);
}

//Put the state, der, input or output named "name" in the vector of pointers 
//to be stored. If none with "name" is found the function raises an invalid_argument
//exception.
void  CommonSystemImpl::store(char* name) {
    if (d_ptr->state_scalars.count(name) == 1) {
        d_ptr->storeHandler.store_scalar(name, d_ptr->state_scalars[name].stateValue);
    } else if (d_ptr->der_scalars.count(name) == 1) {
        d_ptr->storeHandler.store_scalar(name, d_ptr->der_scalars[name].derValue);
    } else if (d_ptr->input_scalars.count(name) == 1) {
        d_ptr->storeHandler.store_scalar(name, d_ptr->input_scalars[name]);
    } else if (d_ptr->output_scalars.count(name) == 1) {
        d_ptr->storeHandler.store_scalar(name, d_ptr->output_scalars[name]);
    } else if (d_ptr->state_vectors.count(name) == 1) {
        d_ptr->storeHandler.store_vector(name, d_ptr->state_vectors[name].stateValue);
    } else if (d_ptr->der_vectors.count(name) == 1) {
        d_ptr->storeHandler.store_vector(name, d_ptr->der_vectors[name].derValue);
    } else if (d_ptr->input_vectors.count(name) == 1) {
        d_ptr->storeHandler.store_vector(name, d_ptr->input_vectors[name]);
    } else if (d_ptr->output_vectors.count(name) == 1) {
        d_ptr->storeHandler.store_vector(name, d_ptr->output_vectors[name]);
    } else {
        char errmsg[50];
        snprintf(errmsg, sizeof(errmsg), "Could not store: %s, no such variable", name);
        throw std::invalid_argument(errmsg);
    }
}

void CommonSystemImpl::add_compare_greater(char* comparename, double comparevalue) {
    using std::make_pair;

    if (d_ptr->output_scalars.count(comparename) == 1) {
        auto p = make_pair(d_ptr->output_scalars[comparename], comparevalue);
        d_ptr->compare_greater_vector.push_back(p);
    } else if (d_ptr->state_scalars.count(comparename) == 1) {
        auto p = make_pair(d_ptr->state_scalars[comparename].stateValue, comparevalue);
        d_ptr->compare_greater_vector.push_back(p);
    } else {
        std::string errtxt("Could not find state or output to use for comparison");
        throw std::invalid_argument(errtxt);
    }
}

void CommonSystemImpl::add_compare_smaller(char* comparename, double comparevalue) {
    using std::make_pair;

    if (d_ptr->output_scalars.count(comparename) == 1) {
        auto p = make_pair(d_ptr->output_scalars[comparename], comparevalue);
        d_ptr->compare_smaller_vector.push_back(p);
    } else if (d_ptr->state_scalars.count(comparename) == 1) {
        auto p = make_pair(d_ptr->state_scalars[comparename].stateValue, comparevalue);
        d_ptr->compare_smaller_vector.push_back(p);
    } else {
        std::string errtxt("Could not find state or output to use for comparison");
        throw std::invalid_argument(errtxt);
    }
}


bool CommonSystemImpl::do_comparison() {
    bool is_greater = false;
    auto compare_greater_pairs = d_ptr->compare_greater_vector.begin();
    while (compare_greater_pairs != d_ptr->compare_greater_vector.end()) {
        if (*(compare_greater_pairs->first) > compare_greater_pairs->second) {
            is_greater = true;
            d_ptr->compare_greater_vector.erase(compare_greater_pairs);
        } else {
            compare_greater_pairs++;
        }
    }

    bool is_smaller = false;
    auto compare_smaller_pairs = d_ptr->compare_smaller_vector.begin();
    while (compare_smaller_pairs != d_ptr->compare_smaller_vector.end()) {
        if (*(compare_smaller_pairs->first) < compare_smaller_pairs->second) {
            is_smaller = true;
            d_ptr->compare_smaller_vector.erase(compare_smaller_pairs);
        } else {
            compare_smaller_pairs++;
        }
    }

    return is_greater || is_smaller;
}

double CommonSystemImpl::getNextUpdateTime() {
    return d_ptr->nextUpdateTime;
}

bool CommonSystemImpl::getDiscrete(){
    return d_ptr->isDiscrete;
}

StoreHandler* CommonSystemImpl::getStoreHandlerP(){
    return &(d_ptr->storeHandler);
}

