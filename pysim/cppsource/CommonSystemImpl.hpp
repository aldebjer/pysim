#pragma once
#include <vector>
#include <map>

#include "SimulatableSystem.hpp"

struct CommonSystemImplPrivate;

class  CommonSystemImpl :
    public SimulatableSystem
{
public:
    CommonSystemImpl();
    virtual ~CommonSystemImpl();


    std::vector<std::string> getInputVectorNames();
    std::vector<double> getInputVector(char* name);
    void setInputVector(char* name, std::vector<double> value);
    std::map<std::string, std::string> getInputDescriptionMap();

protected:
    std::auto_ptr<CommonSystemImplPrivate> d_ptr;

};

