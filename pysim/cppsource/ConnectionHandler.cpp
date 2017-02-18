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
    std::vector<std::pair<Eigen::MatrixXd*, Eigen::MatrixXd* > > connected_matrices;

    std::vector<std::pair<double*, double* > > connected_scalar_states_;
    std::vector<std::pair<pysim::vector*, pysim::vector* > > connected_vector_states;
    std::vector<std::pair<Eigen::MatrixXd*, Eigen::MatrixXd* > > connected_matrix_states;
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
    } else if (inputsys->inputs.d_ptr->matrices.count(inputname) > 0) {
        if (d_ptr->outputp->d_ptr->matrices.count(outputname) == 1) {
            auto p = make_pair(d_ptr->outputp->d_ptr->matrices[outputname], inputsys->inputs.d_ptr->matrices[inputname]);
            d_ptr->connected_matrices.push_back(p);
        } else if ((d_ptr->statep != nullptr) && (d_ptr->statep->d_ptr->matrices.count(outputname) == 1)) {
            auto p = make_pair(d_ptr->statep->d_ptr->matrices[outputname], inputsys->inputs.d_ptr->matrices[inputname]);
            d_ptr->connected_matrix_states.push_back(p);
        } else if ((d_ptr->derp != nullptr) && (d_ptr->derp->d_ptr->matrices.count(outputname) == 1)) {
            auto p = make_pair(d_ptr->derp->d_ptr->matrices[outputname], inputsys->inputs.d_ptr->matrices[inputname]);
            d_ptr->connected_matrices.push_back(p);
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
    auto copyfunc = [] (auto vi){*(vi.second) = *(vi.first);};
    for_each(d_ptr->connected_scalars.cbegin(),d_ptr->connected_scalars.cend(),copyfunc);
    for_each(d_ptr->connected_vectors.cbegin(),d_ptr->connected_vectors.cend(),copyfunc);
    for_each(d_ptr->connected_matrices.cbegin(),d_ptr->connected_matrices.cend(),copyfunc);
}

void ConnectionHandler::copystateoutputs() {
    auto copyfunc = [] (auto vi){*(vi.second) = *(vi.first);};
    for_each(d_ptr->connected_scalar_states_.cbegin(),d_ptr->connected_scalar_states_.cend(),copyfunc);
    for_each(d_ptr->connected_vector_states.cbegin(),d_ptr->connected_vector_states.cend(),copyfunc);
    for_each(d_ptr->connected_matrix_states.cbegin(),d_ptr->connected_matrix_states.cend(),copyfunc);
}


}//end namespace pysim