#include "Factory.hpp"

#include "VanDerPolSystem.h"
#include "MassSpringDamper.h"
#include "PredatorPrey.h"
#include "SquareWave.h"
#include "Adder3D.h"
#include "ScalarAdder.h"
#include "DiscretePID.h"
#include "ReadTextInput.h"
#include "RigidBody.h"

#include <cstring>

using namespace std;

CppSystem* getCppSystem(char* name)
{
    if (strcmp(name,"VanDerPol")==0){
        return new VanDerPolSystem();
    }else if(strcmp(name,"MassSpringDamper")==0){
        return new MassSpringDamper();
    }else if (strcmp(name, "PredatorPrey") == 0){
        return new PredatorPrey();
    }else if (strcmp(name, "SquareWave") == 0){
        return new SquareWave();
    }else if (strcmp(name, "Adder3D") == 0){
        return new Adder3D();
    } else if (strcmp(name, "ScalarAdder") == 0) {
        return new ScalarAdder();
    }else if (strcmp(name, "DiscretePID") == 0){
        return new DiscretePID();
    }else if (strcmp(name, "ReadTextInput") == 0){
        return new ReadTextInput();
    }else if (strcmp(name, "RigidBody") == 0) {
        return new RigidBody();
    }else{
        return nullptr;
    }
}

string getCppSystemDocs(char* name)
{
    if (strcmp(name, "VanDerPol") == 0){
        return VanDerPolSystem::getDocs();
    }else if (strcmp(name, "PredatorPrey") == 0){
        return PredatorPrey::getDocs();
    } else if (strcmp(name, "ScalarAdder") == 0) {
        return PredatorPrey::getDocs();
    }else{
        return "";
    }
}



vector<string> getCppSystemNames()
{
    vector<string> names;
    names.push_back("VanDerPol");
    names.push_back("MassSpringDamper");
    names.push_back("PredatorPrey");
    names.push_back("SquareWave");
    names.push_back("Adder3D");
    names.push_back("ScalarAdder");
    names.push_back("DiscretePID");
    names.push_back("ReadTextInput");
    names.push_back("RigidBody");
    return names;
}