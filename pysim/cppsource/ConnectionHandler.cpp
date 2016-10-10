#define _SCL_SECURE_NO_WARNINGS

#include "ConnectionHandler.hpp"


#include <memory>
#include <boost/algorithm/string.hpp>
#include <vector>
#include <string>

#include "CommonSystemImpl_p.hpp"
#include "CompositeSystemImpl_p.hpp"
#include "Variable_p.hpp"

using std::string;
using std::shared_ptr;
using std::map;

namespace pysim {

struct ConnectionHandlerPrivate {
    Variable* outputp;
    Variable* statep;
    Variable* derp;

    std::vector<std::pair<double*, double* > > connected_scalars;
    std::vector<std::pair<pysim::vector*, pysim::vector* > > connected_vectors;

    std::vector<std::pair<double*, double* > > connected_scalar_states_;
    std::vector<std::pair<pysim::vector*, pysim::vector* > > connected_vector_states;
};

ConnectionHandler::ConnectionHandler(Variable* outputp, Variable* statep, Variable* derp):
    d_ptr(new ConnectionHandlerPrivate())
{
    d_ptr->outputp = outputp;
    d_ptr->statep = statep;
    d_ptr->derp = derp;
}

ConnectionHandler::~ConnectionHandler() {
};

template <typename T>
void ConnectionHandler::connect(char* outputname, T* inputsys, char* inputname) {
    using std::make_pair;
    if (inputsys->inputs.d_ptr->scalars.count(inputname) > 0) {
        if (d_ptr->outputp->d_ptr->scalars.count(outputname) == 1) {
            auto p = make_pair(d_ptr->outputp->d_ptr->scalars[outputname], inputsys->inputs.d_ptr->scalars[inputname]);
            d_ptr->connected_scalars.push_back(p);
        } else if ((d_ptr->statep != nullptr) && (d_ptr->statep->d_ptr->scalars.count(outputname) == 1)) {
            auto p = make_pair(d_ptr->statep->d_ptr->scalars[outputname], inputsys->inputs.d_ptr->scalars[inputname]);
            d_ptr->connected_scalar_states_.push_back(p);
        } else if ((d_ptr->derp != nullptr) && (d_ptr->derp->d_ptr->scalars.count(outputname) == 1)) {
            auto p = make_pair(d_ptr->derp->d_ptr->scalars[outputname], inputsys->inputs.d_ptr->scalars[inputname]);
            d_ptr->connected_scalars.push_back(p);
        } else {
            std::string errtxt("Could not find matching state, der, or output to connect from");
            throw std::invalid_argument(errtxt);
        }
    } else if (inputsys->inputs.d_ptr->vectors.count(inputname) > 0) {
        if (d_ptr->outputp->d_ptr->vectors.count(outputname) == 1) {
            auto p = make_pair(d_ptr->outputp->d_ptr->vectors[outputname], inputsys->inputs.d_ptr->vectors[inputname]);
            d_ptr->connected_vectors.push_back(p);
        } else if ((d_ptr->statep != nullptr) && (d_ptr->statep->d_ptr->vectors.count(outputname) == 1)) {
            auto p = make_pair(d_ptr->statep->d_ptr->vectors[outputname], inputsys->inputs.d_ptr->vectors[inputname]);
            d_ptr->connected_vector_states.push_back(p);
        } else if ((d_ptr->derp != nullptr) && (d_ptr->derp->d_ptr->vectors.count(outputname) == 1)) {
            auto p = make_pair(d_ptr->derp->d_ptr->vectors[outputname], inputsys->inputs.d_ptr->vectors[inputname]);
            d_ptr->connected_vectors.push_back(p);
        } else {
            std::string errtxt("Could not find matching state, der or output to connect from");
            throw std::invalid_argument(errtxt);
        }
    } else {
        throw std::invalid_argument("Could not find input to connect to");
    }
}

template void ConnectionHandler::connect<CommonSystemImpl>(char* outputname, CommonSystemImpl* inputsys, char* inputname);
template void ConnectionHandler::connect<CompositeSystemImpl>(char* outputname, CompositeSystemImpl* inputsys, char* inputname);

template <typename T>
void ConnectionHandler::connect(char* outputname, T* inputsys, char* inputname, int output_index) {
    using std::make_pair;
    if (inputsys->inputs.d_ptr->scalars.count(inputname) > 0) {
        if (d_ptr->outputp->d_ptr->vectors.count(outputname) == 1) {
            pysim::vector* v_ptr = d_ptr->outputp->d_ptr->vectors[outputname];
            double* element_ptr = &(v_ptr->data()[output_index]);
            auto p = make_pair(element_ptr, inputsys->inputs.d_ptr->scalars[inputname]);
            d_ptr->connected_scalars.push_back(p);
        } else if ((d_ptr->statep != nullptr) && (d_ptr->statep->d_ptr->vectors.count(outputname) == 1)) {
            pysim::vector* v_ptr = d_ptr->statep->d_ptr->vectors[outputname];
            double* element_ptr = &(v_ptr->data()[output_index]);
            auto p = make_pair(element_ptr, inputsys->inputs.d_ptr->scalars[inputname]);
            d_ptr->connected_scalars.push_back(p);
        } else if ((d_ptr->derp != nullptr) && (d_ptr->derp->d_ptr->vectors.count(outputname) == 1)) {
            pysim::vector* v_ptr = d_ptr->derp->d_ptr->vectors[outputname];
            double* element_ptr = &(v_ptr->data()[output_index]);
            auto p = make_pair(element_ptr, inputsys->inputs.d_ptr->scalars[inputname]);
            d_ptr->connected_scalars.push_back(p);
        } else {
            std::string errtxt("Could not find matching state, der or output to connect from");
            throw std::invalid_argument(errtxt);
        }
    } else {
        throw std::invalid_argument("Could not find input to connect to");
    }
}

template void ConnectionHandler::connect<CommonSystemImpl>(char* outputname, CommonSystemImpl* inputsys, char* inputname, int output_index);
template void ConnectionHandler::connect<CompositeSystemImpl>(char* outputname, CompositeSystemImpl* inputsys, char* inputname, int output_index);

void ConnectionHandler::copyoutputs() {
    for (auto vi = d_ptr->connected_scalars.cbegin(); vi != d_ptr->connected_scalars.cend(); ++vi) {
        *(vi->second) = *(vi->first);
    }

    for (auto vi = d_ptr->connected_vectors.cbegin(); vi != d_ptr->connected_vectors.cend(); ++vi) {
        *(vi->second) = *(vi->first);
    }
}

void ConnectionHandler::copystateoutputs() {
    for (auto vi = d_ptr->connected_scalar_states_.cbegin(); vi != d_ptr->connected_scalar_states_.cend(); ++vi) {
        *(vi->second) = *(vi->first);
    }

    for (auto vi = d_ptr->connected_vector_states.cbegin(); vi != d_ptr->connected_vector_states.cend(); ++vi) {
        *(vi->second) = *(vi->first);
    }
}


}//end namespace pysim