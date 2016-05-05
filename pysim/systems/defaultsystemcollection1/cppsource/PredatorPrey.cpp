#include "PredatorPrey.h"

#include "factory.hpp"

std::string PredatorPrey::getDocs(){
    return std::string(
        "System representing the Predator Prey equations for population dynamics\n\n"

        "The predator-prey equations, are a pair of first order, "
        "non-linear, differential equations frequently used to describe the "
        "dynamics of biological systems in which two species interact, one as "
        "a predator and the other as prey. The populations change through time "
        "according to the pair of equations :"
        "$$\\\\frac{dx}{dt} = x(\\\\alpha-\\\\beta y)$$\n"
        "$$\\\\frac{dy}{dt} = -y(\\\\gamma-\\\\delta x)$$\n"
        "where x is the number of prey, y is the number of predators, and alpha, "
        "beta, gamme, delta are parameters describing the interaction.\n\n"
        "For further information see: http://en.wikipedia.org/wiki/Lotka%E2%80%93Volterra_equation"
        );
}

PredatorPrey::PredatorPrey(void)
{
    INPUT(alfa,"Exponential growth constant")
    INPUT(beta, "Constand for rate of predation upon prey")
    INPUT(gamma, "Loss rate of predators")
    INPUT(delta,"Growth constant for predator population")
    INPUT(x0, "Initial prey population")
    INPUT(y0, "Initial predator population")

    STATE(x,dx,"Number of Prey")
    STATE(y,dy, "Number of Predators")

	x0 = 1; //number of animals of type X
	y0 = 2; //number of animals of type Y
	
    alfa = 1.5;
    beta = 1;
    gamma = 3;
    delta = 1;
}

void PredatorPrey::doStep(double time){
    dx = x*(alfa-beta*y);
    dy = -y*(gamma-delta*x);
}

REGISTER_SYSTEM(PredatorPrey);
