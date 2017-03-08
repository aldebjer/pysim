#include "factory.hpp"

#include <stdexcept>

using namespace std;

pysim::CppSystem* getCppSystem(const char* name) {
    return SystemFactory::Instance().Create(name);
}

string getCppSystemDocs(const char* name) {
    return SystemFactory::Instance().getDocs(name);
}

vector<string> getCppSystemNames() {
    return SystemFactory::Instance().getNames();
}



SystemFactory& SystemFactory::Instance()
{
    static SystemFactory factory;
    return factory;
}

void SystemFactory::RegisterMaker(const std::string& key, ISystemMaker* maker)
{
    if (_makers.find(key) != _makers.end())
    {
        throw new std::runtime_error("Multiple makers for given key!");
    }
    _makers[key] = maker;
}

pysim::CppSystem* SystemFactory::Create(const char* name) const
{
    string key(name);
    auto i = _makers.find(key);
    if (i == _makers.end()) {
        throw new runtime_error("Unrecognized object type!");
    }
    ISystemMaker* maker = i->second;
    return maker->Create();
}

std::string SystemFactory::getDocs(const char* name) const
{
    string key(name);
    auto i = _makers.find(key);
    if (i == _makers.end()) {
        throw new runtime_error("Unrecognized object type!");
    }
    ISystemMaker* maker = i->second;
    return maker->getDocs();
}

std::vector<string> SystemFactory::getNames() {
    vector<string> names;
    for (auto iter = _makers.begin(); iter != _makers.end(); ++iter) {
        names.push_back(iter->first);
    }
    return names;
}
