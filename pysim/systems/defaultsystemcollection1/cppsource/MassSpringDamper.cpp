#include "MassSpringDamper.h"

#include "factory.hpp"

std::string MassSpringDamper::getDocs(){
    return std::string("TODO: Add this");
}


MassSpringDamper::MassSpringDamper(void)
{
    INPUT(m, "Mass")
    INPUT(b, "Damping constant")
    INPUT(k, "Spring constant")
    INPUT(f, "External force acting on the mass")
    
    STATE(x1,dx1,"Position")
    STATE(x2,dx2, "Velocity")

    OUTPUT(acceleration, "Acceleration of the mass")
    
    
    x1 = 1;
    x2 = 0;
    m = 100;
    b = 1;
    k = 50;
    f = 0;
}

void MassSpringDamper::doStep(double time){
    dx1 = x2;
    acceleration = -k / m*x1 - b / m*x2 + 1 / m*f;
    dx2 = acceleration;
}

REGISTER_SYSTEM(MassSpringDamper);