#include "factory.hpp"

using namespace std;

CppSystem* getCppSystem(char* name) {
    return SystemFactory::Instance().Create(name);
}

string getCppSystemDocs(char* name) {
    return "";
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
        throw new exception("Multiple makers for given key!");
    }
    _makers[key] = maker;
}

CppSystem* SystemFactory::Create(char* name) const
{
    string key(name);
    auto i = _makers.find(key);
    if (i == _makers.end()) {
        throw new exception("Unrecognized object type!");
    }
    ISystemMaker* maker = i->second;
    return maker->Create();
}

std::vector<string> SystemFactory::getNames() {
    vector<string> names;
    for (auto iter = _makers.begin(); iter != _makers.end(); ++iter) {
        names.push_back(iter->first);
    }
    return names;
}