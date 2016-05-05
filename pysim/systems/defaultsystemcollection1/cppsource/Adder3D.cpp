#include "Adder3D.h"

#include "factory.hpp"


std::string Adder3D::getDocs(){
    return std::string(
"System that simply adds two vectors together\n\n"
);
}

Adder3D::Adder3D(void) :
    input1(3,0.0),
    input2(3,0.0),
    output1(3,0.0)
{
    INPUT(input1, "First vector to be added")
    INPUT(input2, "Second vector to be added")

    OUTPUT(output1, "Result of the addition")
}

void Adder3D::doStep(double time){
    output1 = input1 + input2;
}

REGISTER_SYSTEM(Adder3D);
