#pragma once
#include <vector>
#include <map>


#include "PysimTypes.hpp"

struct CommonSystemImplPrivate {
    std::map<std::string, pysim::vector* > input_vectors;
    std::map<std::string, std::string> input_descriptions;
};


