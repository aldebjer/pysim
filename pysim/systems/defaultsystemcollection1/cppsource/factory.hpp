#include "CppSystem.hpp"

#include "vector"

#ifdef WIN32
    #ifdef DEFAULTSYSTEMCOLLECTION1_EXPORTS  
        #define DEFAULTSYSTEMCOLLECTION1_API __declspec(dllexport)   
    #else  
        #define DEFAULTSYSTEMCOLLECTION1_API __declspec(dllimport)   
    #endif
#else
    #define DEFAULTSYSTEMCOLLECTION1_API
#endif

extern "C"{
DEFAULTSYSTEMCOLLECTION1_API pysim::CppSystem* __cdecl getCppSystem(char* name);
};
std::string getCppSystemDocs(char* name);

std::vector<std::string> getCppSystemNames();

#define REGISTER_SYSTEM(T) static SystemMaker<T> maker(#T);

class ISystemMaker;

class SystemFactory {
public:
    void RegisterMaker(const std::string& key, ISystemMaker * maker);
    static SystemFactory& Instance();
    pysim::CppSystem* Create(char* name) const;
    std::string getDocs(char* name) const;
    std::vector<std::string> getNames();

private:
    std::map<std::string, ISystemMaker*> _makers;
};

class ISystemMaker {
public:
    virtual pysim::CppSystem * Create() const = 0;
    virtual std::string getDocs() = 0;
    virtual ~ISystemMaker() {}
};

template<typename T>
class SystemMaker : public ISystemMaker {
public:
    SystemMaker(const std::string& key) {
        SystemFactory::Instance().RegisterMaker(key, this);
    }
    pysim::CppSystem * Create() const {
        return new T();
    }
    std::string getDocs(){
        return T::getDocs();
    }
};


