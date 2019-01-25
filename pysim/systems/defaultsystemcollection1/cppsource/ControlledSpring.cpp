#include "ControlledSpring.h"
#include "factory.hpp"

ControlledSpring::ControlledSpring(void):
	amp(0.0),
	out(0.0),
	signal(0.0),
	msd(),
	sqw()
{
	INPUT(amp, "Amplitude");

	OUTPUT(out, "Output");
	OUTPUT(signal, "Signal");

	this->add_subsystem(&msd, "msd");
	this->add_subsystem(&sqw, "sqw");

	msd.inputs.setScalar("b", 80);
	msd.inputs.setScalar("m", 50);
	msd.inputs.setScalar("f", 0);

	sqw.inputs.setScalar("amplitude", 50);
	sqw.inputs.setScalar("freq", 0.1);

	sqw.connectionHandler.connect("signal", this, "signal");

}

std::string ControlledSpring::getDocs() {
	return std::string("");
}

void ControlledSpring::doStep(double time)
{
}

void ControlledSpring::copyinputs()
{
	sqw.inputs.setScalar("amplitude", amp);
}

void ControlledSpring::copyoutputs()
{
	out = msd.states.getScalar("x1");
	//signal = sqw.outputs.getScalar("signal");
}

void ControlledSpring::copystateoutputs()
{
}

REGISTER_SYSTEM(ControlledSpring);