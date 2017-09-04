#include "Adder6D.h"

#include "factory.hpp"
#include <boost/numeric/ublas/assignment.hpp>
#include <boost/format.hpp>

REGISTER_SYSTEM(Adder6D);
using namespace std;

string Adder6D::getDocs() {
	return string(
		"System that simply adds two vectors together\n\n"
		);
}

Adder6D::Adder6D(void) :
	inputs(MAX_INPUTS, pysim::vector(6, 0.0)),
	out(6, 0.0)
{
	int i = 0;
	for (auto iter = inputs.begin(); iter != inputs.end(); ++iter) {
		pysim::vector* p = &(*iter);
		std::string name = str(boost::format("input_%1%") % i++);
		input(p, name.c_str(), "");
	}

	OUTPUT(out, "Result of the addition")
}

void Adder6D::doStep(double time) {
	out <<= 0, 0, 0, 0, 0, 0;
	for (auto iter = inputs.begin(); iter != inputs.end(); ++iter) {
		out += *iter;
	}
}
