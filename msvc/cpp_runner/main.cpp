#include "CppSimulation.hpp"
#include "CppSystem.hpp"
#include "StoreHandler.hpp"

#include <iostream>
#include <windows.h>

typedef pysim::CppSystem*(__cdecl *funcpt)(char* name);

int main(int argc, char *argv[]) {
    HINSTANCE hGetProcIDDLL = LoadLibrary("C:\\dev\\pysims\\pysim\\pysim\\systems\\defaultsystemcollection1.cp35-win32.pyd");
    if (!hGetProcIDDLL) {
        std::cout << "could not load the dynamic library" << std::endl;
        return 1;
    }

    funcpt funci = (funcpt)GetProcAddress(hGetProcIDDLL, "getCppSystem");
    if (!funci) {
        std::cout << "could not locate the function" << std::endl;
        return 1;
    }

    pysim::CppSystem* sys = funci("VanDerPol");
    sys->store("x");
    pysim::Simulation sim;
    sim.addSystem(sys);
    sim.simulate(200, 0.1,"rk4",0.1,0.1,false);
    std::vector<double> v;
    v = sys->getStoreHandlerP()->getStoreVector("x");
    std::cout << v.at(20) << std::endl;

}
