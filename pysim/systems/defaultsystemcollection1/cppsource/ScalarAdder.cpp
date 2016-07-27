#include "ScalarAdder.h"

#include "factory.hpp"
#include <boost/format.hpp>

using std::string;

string ScalarAdder::getDocs() {
    std::string desc = str(boost::format("Adds inputs together. \n There can, at most, be %1% number of inputs, denoted input_0, input_1, ...") % MAX_INPUTS);
    return desc;
}

ScalarAdder::ScalarAdder(void) :
    inputs(MAX_INPUTS,0.0),
    out(0.0)
{
    int i = 0;
    for (auto iter = inputs.begin(); iter != inputs.end(); ++iter) {
        double* p = &(*iter);
        std::string name = str(boost::format("input_%1%") % i++);
        input(p, name.c_str(), "");
    }
    output(&out, "out", "The sum of all inputs");
}

void ScalarAdder::doStep(double time) {
    out = 0;
    for (auto iter = inputs.begin(); iter != inputs.end(); ++iter) {
        out += *iter;
    }
}

REGISTER_SYSTEM(ScalarAdder);
