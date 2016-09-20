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
    input_output_scalar(0.0),
    input_output_vector(3, 0.0),
    state_output_scalar(0.0),
    state_output_vector(3, 0.0),
    state_scalar(1.23),
    state_vector(3, 4.56),
    der_scalar(0.0),
    der_vector(3, 0.0)
{
    INPUT(input_scalar, "Input Scalar")
    INPUT(input_vector, "Input vector")

    STATE(state_scalar, der_scalar, "Scalar state of the test system (constant at 1.23)")
    STATE(state_vector, der_vector, "Vector State of the test system (constant at 4.56)")

    OUTPUT(input_output_scalar, "Value of input scalar")
    OUTPUT(input_output_vector, "Value of input vector")
    OUTPUT(state_output_scalar, "Value of staet scalar")
    OUTPUT(state_output_vector, "Value of state vector")

}

void InOutTestSystem::doStep(double time){
    input_output_scalar = input_scalar;
    input_output_vector = input_vector;
    state_output_scalar = state_scalar;
    state_output_vector = state_vector;
}

REGISTER_SYSTEM(InOutTestSystem);
