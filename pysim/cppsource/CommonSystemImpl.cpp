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

StoreHandler* CommonSystemImpl::getStoreHandlerP(){
    return &(d_ptr->storeHandler);
}

