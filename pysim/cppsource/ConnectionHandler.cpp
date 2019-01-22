#define _SCL_SECURE_NO_WARNINGS

#include "ConnectionHandler.hpp"


#include <memory>
#include <boost/algorithm/string.hpp>
#include <vector>
#include <string>

#include <Eigen/Dense>

#include "CommonSystemImpl_p.hpp"
#include "CompositeSystemImpl_p.hpp"
#include "Variable_p.hpp"

using std::string;
using std::shared_ptr;
using std::map;

namespace pysim {

struct ConnectionHandlerPrivate {
    Variable* inputp;
    Variable* outputp;
    Variable* statep;
    Variable* derp;

    std::vector<std::pair<double*, double* > > connected_scalars_inputs;
    std::vector<std::pair<pysim::vector*, pysim::vector* > > connected_vectors_inputs;
    std::vector<std::pair<Eigen::MatrixXd*, Eigen::MatrixXd* > > connected_matrices_inputs;

    std::vector<std::pair<double*, double* > > connected_scalars_outputs;
    std::vector<std::pair<pysim::vector*, pysim::vector* > > connected_vectors_outputs;
    std::vector<std::pair<Eigen::MatrixXd*, Eigen::MatrixXd* > > connected_matrices_outputs;

    std::vector<std::pair<double*, double* > > connected_scalar_states;
    std::vector<std::pair<pysim::vector*, pysim::vector* > > connected_vector_states;
    std::vector<std::pair<Eigen::MatrixXd*, Eigen::MatrixXd* > > connected_matrix_states;
};

ConnectionHandler::ConnectionHandler(
    Variable* inputp,
    Variable* outputp, 
    Variable* statep, 
    Variable* derp):
    d_ptr(new ConnectionHandlerPrivate())
{
    d_ptr->inputp = inputp;
    d_ptr->outputp = outputp;
    d_ptr->statep = statep;
    d_ptr->derp = derp;
}

ConnectionHandler::~ConnectionHandler() {
};

template <typename T>
void make_connection(std::vector<std::pair<T, T>>* connections, T input, T output) {
    connections->push_back(std::make_pair<T, T>(output, input));
}

void ConnectionHandler::connect(char* outputname, CommonSystemImpl* inputsys, char* inputname) {

    auto inputsys_dptr = inputsys->inputs.d_ptr.get();

    auto input_dptr = this->d_ptr->inputp->d_ptr.get();
    auto output_dptr = this->d_ptr->outputp->d_ptr.get();
    auto state_dptr = this->d_ptr->statep->d_ptr.get();
    auto der_dptr = this->d_ptr->derp->d_ptr.get();

    if (inputsys_dptr->scalars.count(inputname) > 0) {
        // Scalar connection
        if (input_dptr->scalars.count(outputname) > 0) {
            // Input - Input connection
            make_connection(
                &(d_ptr->connected_scalars_inputs),
                inputsys_dptr->scalars[inputname],
                input_dptr->scalars[outputname]);
            return;
        }
        else if (output_dptr->scalars.count(outputname) > 0) {
            // Output - Input connection
            make_connection(
                &(d_ptr->connected_scalars_outputs),
                inputsys_dptr->scalars[inputname],
                output_dptr->scalars[outputname]);
            return;
        }
        else if (der_dptr->scalars.count(outputname) > 0) {
            // Der - Input connection
            make_connection(
                &(d_ptr->connected_scalars_outputs),
                inputsys_dptr->scalars[inputname],
                der_dptr->scalars[outputname]);
            return;
        }
        else if (state_dptr->scalars.count(outputname) > 0) {
            // State - Input connection
            make_connection(
                &(d_ptr->connected_scalar_states),
                inputsys_dptr->scalars[inputname],
                der_dptr->scalars[outputname]);
            return;
        }

        throw std::invalid_argument("Could not find input|output|state|der to connect from");
    }
    else if (inputsys_dptr->vectors.count(inputname) > 0) {
        // Vector connection
        if (input_dptr->vectors.count(outputname) > 0) {
            // Input - Input connection
            make_connection(
                &(d_ptr->connected_vectors_inputs),
                inputsys_dptr->vectors[inputname],
                input_dptr->vectors[outputname]);
            return;
        }
        else if (output_dptr->vectors.count(outputname) > 0) {
            // Output - Input connection
            make_connection(
                &(d_ptr->connected_vectors_outputs),
                inputsys_dptr->vectors[inputname],
                output_dptr->vectors[outputname]);
            return;
        }
        else if (der_dptr->vectors.count(outputname) > 0) {
            // Der - Input connection
            make_connection(
                &(d_ptr->connected_vectors_outputs),
                inputsys_dptr->vectors[inputname],
                der_dptr->vectors[outputname]);
            return;
        }
        else if (state_dptr->vectors.count(outputname) > 0) {
            // State - Input connection
            make_connection(
                &(d_ptr->connected_vector_states),
                inputsys_dptr->vectors[inputname],
                der_dptr->vectors[outputname]);
            return;
        }

        throw std::invalid_argument("Could not find input|output|state|der to connect from");
    }
    else if (inputsys_dptr->matrices.count(inputname) > 0) {
        // Matrix connection
        if (input_dptr->matrices.count(outputname) > 0) {
            // Input - Input connection
            make_connection(
                &(d_ptr->connected_matrices_inputs),
                inputsys_dptr->matrices[inputname],
                input_dptr->matrices[outputname]);
            return;
        }
        else if (output_dptr->matrices.count(outputname) > 0) {
            // Output - Input connection
            make_connection(
                &(d_ptr->connected_matrices_outputs),
                inputsys_dptr->matrices[inputname],
                output_dptr->matrices[outputname]);
            return;
        }
        else if (der_dptr->matrices.count(outputname) > 0) {
            // Der - Input connection
            make_connection(
                &(d_ptr->connected_matrices_outputs),
                inputsys_dptr->matrices[inputname],
                der_dptr->matrices[outputname]);
            return;
        }
        else if (state_dptr->matrices.count(outputname) > 0) {
            // State - Input connection
            make_connection(
                &(d_ptr->connected_matrix_states),
                inputsys_dptr->matrices[inputname],
                der_dptr->matrices[outputname]);
            return;
        }

        throw std::invalid_argument("Could not find input|output|state|der to connect from");

    }

    throw std::invalid_argument("Could not find input to connect to");
}


void check_copy(std::pair<double *,double *> vi){
    if (std::isnan(*(vi.first))){
        throw std::runtime_error("Output from system is NaN");
    }
    *(vi.second) = *(vi.first);
}

void copy(std::pair<pysim::vector *,pysim::vector *> vi){
    *(vi.second) = *(vi.first);
}

void check_copy(std::pair<Eigen::MatrixXd *,Eigen::MatrixXd *> vi){
    if (vi.first->hasNaN()){
        throw std::runtime_error("Output from system is NaN");
    }
    *(vi.second) = *(vi.first);
}

void ConnectionHandler::copyinputs(){
    for (auto connection : d_ptr->connected_scalars_inputs) {
        check_copy(connection);
    }
    for (auto connection : d_ptr->connected_vectors_inputs) {
        copy(connection);
    }
    for (auto connection : d_ptr->connected_matrices_inputs) {
        check_copy(connection);
    }
}

void ConnectionHandler::copyoutputs() {
    for( auto connection: d_ptr->connected_scalars_outputs){
        check_copy(connection);
    }
    for( auto connection: d_ptr->connected_vectors_outputs){
        copy(connection);
    }
    for( auto connection: d_ptr->connected_matrices_outputs){
        check_copy(connection);
    }
}

void ConnectionHandler::copystateoutputs() {
    for( auto connection: d_ptr->connected_scalar_states){
        check_copy(connection);
    }
    for( auto connection: d_ptr->connected_vector_states){
        copy(connection);
    }
    for( auto connection: d_ptr->connected_matrix_states){
        check_copy(connection);
    }
}


}//end namespace pysim