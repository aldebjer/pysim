#include "InOutTestSystem.h"

#include "factory.hpp"


std::string InOutTestSystem::getDocs(){
    return std::string(
"System used to test inputs and outputs inputs\n\n"
);
}

InOutTestSystem::InOutTestSystem(void):
    input_scalar(0.0),
    input_vector(3, 0.0),
    input_matrix(Eigen::MatrixXd::Zero(3, 3)),
    input_output_scalar(0.0),
    input_output_vector(3, 0.0),
    input_output_matrix(Eigen::MatrixXd::Zero(3, 3)),
    state_output_scalar(0.0),
    state_output_vector(3, 0.0),
    state_output_matrix(Eigen::MatrixXd::Zero(3, 3)),
    state_scalar(1.23),
    state_vector(3, 4.56),
    state_matrix(Eigen::MatrixXd::Zero(3, 3)),
    der_scalar(0.0),
    der_vector(3, 0.0),
    der_matrix(Eigen::MatrixXd::Zero(3, 3))
{
    INPUT(input_scalar, "Input Scalar")
    INPUT(input_vector, "Input vector")
    INPUT(input_matrix, "Input matrix")

    STATE(state_scalar, der_scalar, "Scalar state of the test system (constant at 1.23)")
    STATE(state_vector, der_vector, "Vector State of the test system (constant at 4.56)")
    STATE(state_matrix, der_matrix, "Vector State of the test system (constant at 0.0)")

    OUTPUT(input_output_scalar, "Value of input scalar")
    OUTPUT(input_output_vector, "Value of input vector")
    OUTPUT(input_output_matrix, "Value of input matrix")

    OUTPUT(state_output_scalar, "Value of staet scalar")
    OUTPUT(state_output_vector, "Value of state vector")
    OUTPUT(state_output_matrix, "Value of state matrix")
}

void InOutTestSystem::doStep(double time){
    input_output_scalar = input_scalar;
    input_output_vector = input_vector;
    input_output_matrix = input_matrix;
    state_output_scalar = state_scalar;
    state_output_vector = state_vector;
    state_output_matrix = state_matrix;
}

REGISTER_SYSTEM(InOutTestSystem);
