#include "ScalarAdder.h"

#include "factory.hpp"

using std::string;

string ScalarAdder::getDocs() {
    return string(
        "System that simply adds a number of scalar inputs together\n\n"
        );
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
