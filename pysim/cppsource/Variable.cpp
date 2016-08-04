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

std::map<std::string, std::string> Variable::getDescriptionMap() {
    return d_ptr->descriptions;
}

}