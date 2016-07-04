#include "Adder.h"

#include "factory.hpp"


std::string Adder::getDocs(){
    return std::string(
"System that simply adds two scalars together\n\n"
);
}

Adder::Adder(void) :
    input1(0.0),
    input2(0.0),
    output1(0.0)
{
    INPUT(input1, "First scalar to be added")
    INPUT(input2, "Second scalar to be added")

    OUTPUT(output1, "Result of the addition")
}

void Adder::doStep(double time){
    output1 = input1 + input2;
}

REGISTER_SYSTEM(Adder);
