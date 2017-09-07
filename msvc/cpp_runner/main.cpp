#include "CppSimulation.hpp"
#include "CppSystem.hpp"
#include "StoreHandler.hpp"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <sstream>
#include <boost/algorithm/string/replace.hpp>

#include <iostream>
#include <windows.h>

namespace pt = boost::property_tree;

typedef pysim::CppSystem*(__cdecl *funcpt)(const char* name);

struct systeminfo {
    std::string name;
    std::string type;
    std::string module;
    std::vector<std::pair<std::string, std::double_t>> inputs;
    std::vector<std::string> stores;
};

int main(int argc, char *argv[]) {
    using std::string;
    using std::pair;

    string simfilename;
    string modulerootpath;
    if (argc < 3) {
        std::cout << "warning: using default filename and modulepath" << std::endl;
        simfilename = "simplesim.json";
        modulerootpath = "C:\\dev\\pysims\\pysim\\";
    } else {
        simfilename = string(argv[1]);
        modulerootpath = string(argv[2]);
    }

    pt::ptree root;
    pt::read_json(simfilename, root);

    std::vector<systeminfo> system_infos;

    for (pt::ptree::value_type &json_system : root.get_child("systems")) {
        systeminfo sys;
        sys.name = json_system.first;
        sys.type = json_system.second.get_child("type").data();
        sys.module = json_system.second.get_child("module").data();
        for (pt::ptree::value_type & json_input : json_system.second.get_child("inputs")) {
            string key = json_input.first;
            string value(json_input.second.data());
            sys.inputs.push_back(std::make_pair(key, std::stod(value)));
        }
        for (pt::ptree::value_type & json_store : json_system.second.get_child("stores")) {
            sys.stores.push_back(json_store.second.data());
        }
        system_infos.push_back(sys);
    }

    std::map<string, funcpt> factorymap;
    for (systeminfo &sys : system_infos) {
        if (factorymap.count(sys.module) == 0) {
            std::stringstream ss;
            ss << modulerootpath;
            string module(sys.module);
            boost::algorithm::replace_all(module, ".", "\\");
            ss << module << ".cp36-win_amd64.pyd";
            HINSTANCE hGetProcIDDLL = LoadLibrary(ss.str().c_str());
            if (!hGetProcIDDLL) {
                int errorcode = GetLastError();
                std::cout << "could not load the dynamic library: " << errorcode << std::endl;
                return 1;
            }
            funcpt funci = (funcpt)GetProcAddress(hGetProcIDDLL, "getCppSystem");
            if (!funci) {
                std::cout << "could not locate the function" << std::endl;
                return 1;
            }
            factorymap[sys.module] = funci;
        }
    }

    pysim::Simulation sim;
    std::map<string, pysim::CppSystem*> systems;

    for (systeminfo& sys_info : system_infos) {
        pysim::CppSystem* sys = factorymap[sys_info.module](sys_info.type.c_str());
        for (std::string name: sys_info.stores){
            sys->store(name.c_str());
        }
        systems[sys_info.name] = sys;
        sim.addSystem(sys);
    }


    sim.simulate(200, 0.1,"rk4",0.1,0.1,false);
    std::vector<double> v;
    pysim::CppSystem* sys = systems["vanderpol"];
    v = sys->getStoreHandlerP()->getStoreVector("x");
    std::cout << v.at(20) << std::endl;

}
