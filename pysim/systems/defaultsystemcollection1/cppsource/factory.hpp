#include "CppSystem.hpp"

#include "vector"

pysim::CppSystem* getCppSystem(char* name);
std::string getCppSystemDocs(char* name);

std::vector<std::string> getCppSystemNames();

#define REGISTER_SYSTEM(T) static SystemMaker<T> maker(#T);

class ISystemMaker;

class SystemFactory {
public:
    void RegisterMaker(const std::string& key, ISystemMaker * maker);
    static SystemFactory& Instance();
    pysim::CppSystem* Create(char* name) const;
    std::vector<std::string> getNames();

private:
    std::map<std::string, ISystemMaker*> _makers;
};

class ISystemMaker {
public:
    virtual pysim::CppSystem * Create() const = 0;
    virtual ~ISystemMaker() {}
};

template<typename T>
class SystemMaker : public ISystemMaker {
public:
    SystemMaker(const std::string& key) {
        SystemFactory::Instance().RegisterMaker(key, this);
    }
    virtual pysim::CppSystem * Create() const {
        return new T();
    }
};


