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

template<class T>
std::map<std::string, T> get_output_map(VariablePrivate* var) {
    std::map<std::string, T> mymap;
    return mymap;
}
template<>
std::map<std::string, double*> get_output_map<double*>(VariablePrivate* var) {
    return var->scalars;
}
template<>
std::map<std::string, pysim::vector*> get_output_map<pysim::vector*>(VariablePrivate* var) {
    return var->vectors;
}
template<>
std::map<std::string, Eigen::MatrixXd*> get_output_map<Eigen::MatrixXd*>(VariablePrivate* var) {
    return var->matrices;
}

//Overloaded function 'get_connection' for getting the correect variable for each type
template<class T>
std::vector<std::pair<T, T > >* get_connections(ConnectionHandlerPrivate* var) {
    std::vector<std::pair<T, T > > connected;
    return nullptr;
}
template<>
std::vector<std::pair<double*, double* > >* get_connections(ConnectionHandlerPrivate* var) {
    return &var->connected_scalars;
}
template<>
std::vector<std::pair<pysim::vector*, pysim::vector* > >* get_connections(ConnectionHandlerPrivate* var) {
    return &var->connected_vectors;
}
template<>
std::vector<std::pair<Eigen::MatrixXd*, Eigen::MatrixXd* > >* get_connections(ConnectionHandlerPrivate* var) {
    return &var->connected_matrices;
}

template <typename T>
bool ConnectionHandler::check_input(std::map<std::string, T > input, char* inputname, char* outputname) {
    if (input.count(inputname) > 0) {

        //Create a vector vec with all possible sources: outputs, states and ders
        std::vector<VariablePrivate*> vec;
        vec.push_back(d_ptr->outputp->d_ptr.get());
        if (d_ptr->statep != nullptr) vec.push_back(d_ptr->statep->d_ptr.get());
        if (d_ptr->derp != nullptr) vec.push_back(d_ptr->derp->d_ptr.get());

        //For each possible source check if there is a corresponding name. If so create the connection.
        for (auto v : vec) {
            std::map<std::string, T> output = get_output_map<T>(v);
            if (output.count(outputname) > 0) {
                std::vector<std::pair<T, T > >* connections = get_connections<T>(d_ptr.get());
                auto p = std::make_pair(output[outputname],input[inputname]);
                connections->push_back(p);
                return true;
            }
        }
    }
    return false;
}

template <typename T>
void ConnectionHandler::connect(char* outputname, T* inputsys, char* inputname) {
    if (check_input(inputsys->inputs.d_ptr->scalars, inputname, outputname)) {
        return;
    } else if (check_input(inputsys->inputs.d_ptr->vectors, inputname, outputname)) {
        return;
    }else if (check_input(inputsys->inputs.d_ptr->matrices, inputname, outputname)) {
        return;
    }
    throw std::invalid_argument("Could not find input to connect to");

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

void ConnectionHandler::copyoutputs() {
    for( auto connection: d_ptr->connected_scalars){
        check_copy(connection);
    }
    for( auto connection: d_ptr->connected_vectors){
        copy(connection);
    }
    for( auto connection: d_ptr->connected_matrices){
        check_copy(connection);
    }
}

void ConnectionHandler::copystateoutputs() {
    for( auto connection: d_ptr->connected_scalar_states_){
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