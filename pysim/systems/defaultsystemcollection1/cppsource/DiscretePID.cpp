#include "DiscretePID.h"

#include <cmath>
#include <algorithm>

#include "factory.hpp"

std::string DiscretePID::getDocs(){
    return std::string("TODO: Add this");
}


DiscretePID::DiscretePID(void)
{
    INPUT(stepsize,"stepsize for the controller")
    INPUT(p,"Proportial constant")
    INPUT(plim, "Limit for the proportional part of the error")
    INPUT(i, "Integration constant")
    INPUT(d, "Derivative constant")
    INPUT(antiwindup, "Antiwindup (1=active, 0=Inactive)")

    INPUT(insig, "Input signal")
    INPUT(refsig, "Set signal")
    INPUT(dsig, "Derivative of input signal")

    STATE(iPart, dIPart,"Integrated part")

    OUTPUT(outsig, "Control signal coming from the controller")
    stepsize = 0.5;
    antiwindup = 0;
    p = 1;
    i = 0;
    d = 0;
    dsig = 0;
    outsig = 0;

    plim = 1;
    refsig = 0;
    insig = 0;

    iPart = 0;
    dIPart = 0;
    setDiscrete();
    setNextUpdate(stepsize);
}

void DiscretePID::preSim(){
}

void DiscretePID::doStep(double time){
    double err = refsig - insig;
    double v = p*err + iPart*i+dsig*d;
    if (v >= plim){
        outsig = plim;
        dIPart = iPart + err*antiwindup;
    }
    else if (v <= -1 * plim){
        outsig = -1 * plim;
        dIPart = iPart + err*antiwindup;
    }
    else{
        outsig = v;
        dIPart = iPart + err;
    }

    setNextUpdate(time + stepsize);
}

REGISTER_SYSTEM(DiscretePID);
