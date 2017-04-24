#include "Variable.hpp"

#include <boost/algorithm/string.hpp>
#include <iostream>
#include <string>
#include <boost/format.hpp>

#include "PysimTypes.hpp"

#include "Variable_p.hpp"

using std::string;

namespace pysim {


Variable::Variable():
    d_ptr(new VariablePrivate())
{

}

std::vector<std::string> Variable::getScalarNames() {
    std::vector<std::string> names;
    for (auto i = d_ptr->scalars.cbegin(); i != d_ptr->scalars.cend(); ++i) {
        names.push_back(i->first);
    }
    return names;
}

std::vector<std::string> Variable::getVectorNames() {
    std::vector<std::string> names;
    for (auto i = d_ptr->vectors.cbegin(); i != d_ptr->vectors.cend(); ++i) {
        names.push_back(i->first);
    }
    return names;
}

std::vector<std::string> Variable::getMatrixNames() {
    std::vector<std::string> names;
    for (auto i = d_ptr->matrices.cbegin(); i != d_ptr->matrices.cend(); ++i) {
        names.push_back(i->first);
    }
    return names;
}

void Variable::setScalar(char* name, double value) {
    if (d_ptr->scalars.count(name) < 1) {
        char errmsg[50];
        snprintf(errmsg, 50, "Could not find: %s", name);
        throw std::invalid_argument(errmsg);
    }
    *(d_ptr->scalars.at(name)) = value;
}

void Variable::setVector(char* name, std::vector<double> value) {
    if (d_ptr->vectors.count(name) > 0) {
        auto bv = d_ptr->vectors[name];
        if (bv->size() != value.size()) {
            std::string errstr = str(boost::format("Size of %1% is %2%") % name % bv->size());
            throw std::invalid_argument(errstr);
        }
        std::copy(value.begin(), value.end(), bv->begin());
    } else {
        std::string errstr = str(boost::format("Could not find: %1%") % name);
        throw std::invalid_argument(errstr);
    }
}

void Variable::setMatrix(char* name, std::vector<std::vector<double>> value) {
    using namespace Eigen;

    //Check that matrix exist
    if (d_ptr->matrices.count(name) <= 0) {
        std::string errstr = str(boost::format("Could not find: %1%") % name);
        throw std::invalid_argument(errstr);
    }

    //Local pointer to matrix
    MatrixXd* mp = d_ptr->matrices[name];

    //Check row size
    if (mp->rows() != (int)value.size()) {
        std::string errstr = str(boost::format("Row size of %1% is %2%") % name % mp->rows());
        throw std::invalid_argument(errstr);
    }

    //Check column sizes
    for (std::vector<double> in_row : value) {
        if (mp->cols() != (int)in_row.size()) {
            std::string errstr = str(boost::format("column size of %1% is %2%") % name % mp->cols());
            throw std::invalid_argument(errstr);
        }
    }

    //Copy values
    size_t current_row = 0;
    for (std::vector<double> in_row : value) {
        size_t current_element = 0;
        for (double element : in_row) {
            mp->operator()(current_row, current_element++) = element;
        }
        ++current_row;
    }
}

double Variable::getScalar(char* name) {
    if (d_ptr->scalars.count(name) < 1) {
        std::string errstr = str(boost::format("Could not find: %1%") % name);
        throw std::invalid_argument(errstr);
    }
    return *(d_ptr->scalars.at(name));
}

std::vector<double> Variable::getVector(char* name) {
    if (d_ptr->vectors.count(name) > 0) {
        auto bv = d_ptr->vectors[name];
        std::vector<double> v(bv->size());
        std::copy(bv->begin(), bv->end(), v.begin());
        return v;
    } else {
        std::string errstr = str(boost::format("Could not find: %1%") % name);
        throw std::invalid_argument(errstr);
    }
}

std::vector<std::vector<double>> Variable::getMatrix(char* name) {
    using namespace Eigen;

    //Check that matrix exist
    if (d_ptr->matrices.count(name) <= 0) {
        std::string errstr = str(boost::format("Could not find: %1%") % name);
        throw std::invalid_argument(errstr);
    }

    //Local pointer to matrix
    MatrixXd* mp = d_ptr->matrices[name];

    std::vector<std::vector<double>> out;
    for (int i = 0; i < mp->rows(); ++i) {
        std::vector<double> out_row;
        for (int j = 0; j < mp->cols(); ++j) {
            out_row.push_back(mp->operator()(i, j));
        }
        out.push_back(out_row);
    }
    return out;
}

std::map<std::string, std::string> Variable::getDescriptionMap() {
    return d_ptr->descriptions;
}

}