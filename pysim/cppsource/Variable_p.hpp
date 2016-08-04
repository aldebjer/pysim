#pragma once
#include <vector>
#include <map>
#include <memory>

#include "Variable.hpp"

#include "PysimTypes.hpp"


namespace pysim {

struct VariablePrivate{
    std::map<std::string, double* > scalars;
    std::map<std::string, pysim::vector* > vectors;
    std::map<std::string, std::string> descriptions;
};

}
