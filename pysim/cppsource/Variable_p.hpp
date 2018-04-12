#pragma once
#include <vector>
#include <map>
#include <memory>

#include "Variable.hpp"

#include "PysimTypes.hpp"

#include "boost/variant.hpp"

#include <Eigen/Dense>

namespace pysim {

struct VariablePrivate{
    std::map<std::string, double* > scalars;
    std::map<std::string, pysim::vector* > vectors;
    std::map<std::string, Eigen::MatrixXd* > matrices;
    std::map<std::string, std::string> descriptions;
	std::map<std::string, std::string> operators;
};

}
