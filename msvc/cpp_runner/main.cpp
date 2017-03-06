#include <iostream>
#include "CppSimulation.hpp"
#include "CppSystem.hpp"
#include "factory.hpp"
#include "StoreHandler.hpp"

int main(int argc, char *argv[]) {
    pysim::CppSystem* sys;
    sys = getCppSystem("VanDerPol");
    sys->store("x");
    pysim::Simulation sim;
    sim.addSystem(sys);
    sim.simulate(200, 0.1,"rk4",0.1,0.1,false);
    std::vector<double> v;
    v = sys->getStoreHandlerP()->getStoreVector("x");
    std::cout << v.at(20) << std::endl;

}
