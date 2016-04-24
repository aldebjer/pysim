#include "MassSpringDamper.h"


MassSpringDamper::MassSpringDamper(void)
{
    INPUT(m, "Mass")
    INPUT(b, "Damping constant")
    INPUT(k, "Spring constant")
    INPUT(f, "External force acting on the mass")
    
    STATE(x1,dx1,"Position")
    STATE(x2,dx2, "Velocity")
    
    
    x1 = 1;
    x2 = 0;
    m = 100;
    b = 1;
    k = 50;
    f = 0;
}

void MassSpringDamper::doStep(double time){
    dx1 = x2;
    dx2 = -k/m*x1-b/m*x2+1/m*f;
}