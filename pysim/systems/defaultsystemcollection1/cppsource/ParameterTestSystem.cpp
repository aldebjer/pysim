#include "ParameterTestSystem.h"

#include "factory.hpp"


std::string ParameterTestSystem::getDocs(){
    return std::string(
"System used to test parameter inputs\n\n"
);
}

ParameterTestSystem::ParameterTestSystem(void)
{
    PAR(parameter_vector, "Parameter vector")
    PAR(parameter_matrix, "Parameter Matrix")
    PAR(parameter_map, "Parameter Map")
    PAR(parameter_vectormap, "Parameter Vector Map")

    OUTPUT(output_from_vector, "Value of first element in parameter_vector")
    OUTPUT(output_from_matrix, "Value of first column, first row in parameter_matrix")
    OUTPUT(output_from_map, "Value of element 'a' in parameter_map")
    OUTPUT(output_from_vectormap, "Value of second element of 'a' in parameter_vectormap")

}

void ParameterTestSystem::doStep(double time){
    output_from_vector = parameter_vector[0];
    output_from_matrix = parameter_matrix[0][0];
    output_from_map = parameter_map["a"];
    output_from_vectormap = parameter_vectormap["a"][1];
}

REGISTER_SYSTEM(ParameterTestSystem);
